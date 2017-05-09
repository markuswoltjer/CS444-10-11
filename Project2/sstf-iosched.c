/*
 * elevator sstf
 */
#include <linux/blkdev.h>
#include <linux/elevator.h>
#include <linux/bio.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/init.h>

struct sstf_data {
	
	struct list_head queue;
	struct list_head* enqueue;
	int queue_count;
};

static void sstf_merged_requests(struct request_queue *q, struct request *rq,
				 struct request *next)
{
	list_del_init(&next->queuelist);
}

static int sstf_dispatch(struct request_queue *q, int force)
{
	struct sstf_data *nd = q->elevator->elevator_data;

	if (!list_empty(&nd->queue)) {
		struct request *rq;
		rq = list_entry(nd->queue.next, struct request, queuelist);
		list_del_init(&rq->queuelist);
		elv_dispatch_sort(q, rq);
		return 1;
	}
	return 0;
}

//The add request function has been modified to add requests accordinng to the CLOOK algorithm
static void sstf_add_request(struct request_queue *q, struct request *req)
{
	//Initialize a data struct for a CLOOK-relevant data struct using the elevator field of the request queue
	struct sstf_data *sd = q->elevator->elevator_data;
	
	//flag for successful request addition
	int added = 0;
	
//	printk("I/O request to add: %lu\n", (unsigned long)blk_rq_pos(req));
	
	if(list_empty(&sd->queue)){//check whether list is empty
//		printk ("List was empty, now will be served one request.\n");
		list_add(&req->queuelist, &sd->queue);//add the list to the queue
		sd->enqueue = sd->queue.next;//single list element, so no concern as to where
		sd->queue_count++;//increment queue element tracker
		return;//that's all needed for empty list
	}
	
	//Otherwise, list already has elements
	struct list_head* link;
	list_for_each(link, &sd->queue) {//iterate through the linked list, 
		struct request* this_req = list_entry(link, struct request, queuelist);
		struct request* next_req = list_entry(link->next, struct request, queuelist);
		
		sector_t this_sector = blk_rq_pos(this_req);
		sector_t next_sector = blk_rq_pos(next_req);
		sector_t req_sector = blk_rq_pos(req);
	
//		printk ("Sector of the current link: %lu. Sector of the next link: %lu\n" , (unsigned long)blk_rq_pos(this_req), (unsigned long)blk_rq_pos(next_req));
	
		//This is for one item in the queue 
		if(sd->queue_count == 1) {
			list_add(&req->queuelist, link);//we can just use a simple add again because there is no order in a resulting list of two elements
			sd->queue_count++;
			added = 1;//set flag
			break;//leave the for loop
		}   
	
		//This is the consequential check for general additions. If the new sector is between the current and next, use the position member 
		if ((this_sector <= req_sector) && (next_sector >= req_sector)) {
			list_add(&req->queuelist, link);
			sd->queue_count++;
			added = 1;//set flag
			break;//leave the for loop
		}
	}
	
	if(added != 1) {//if non suitable position was found between sectors on the list, it must
		// Our new request must be bigger than all current, add it to the end
		list_add_tail(&req->queuelist, &sd->queue);
		sd->queue_count++;
//		printk("High sector request added at tail of queue (beyond the highest sector).\n");
	}
	
//	printk("Added I/O request: %llu\n", (unsigned long long)blk_rq_pos(req));
//	printk("Queue size after addition: %d\n", sd->queue_count);
//	printk("The resulting queue:");

	//Print sectors in order
/*	printk("START QUEUE:\n");
	struct request *cur_req = NULL;
	list_for_each_entry(cur_req, &sd->queue, queuelist)
		printk("%llu\n", (unsigned long long)blk_rq_pos(cur_req));
	printk("END QUEUE.\n\n");
*/
}

static struct request *
sstf_former_request(struct request_queue *q, struct request *rq)
{
	struct sstf_data *nd = q->elevator->elevator_data;

	if (rq->queuelist.prev == &nd->queue)
		return NULL;
	return list_entry(rq->queuelist.prev, struct request, queuelist);
}

static struct request *
sstf_latter_request(struct request_queue *q, struct request *rq)
{
	struct sstf_data *nd = q->elevator->elevator_data;

	if (rq->queuelist.next == &nd->queue)
		return NULL;
	return list_entry(rq->queuelist.next, struct request, queuelist);
}

static int sstf_init_queue(struct request_queue *q, struct elevator_type *e)
{
	struct sstf_data *nd;
	struct elevator_queue *eq;

	eq = elevator_alloc(q, e);
	if (!eq)
		return -ENOMEM;

	nd = kmalloc_node(sizeof(*nd), GFP_KERNEL, q->node);
	if (!nd) {
		kobject_put(&eq->kobj);
		return -ENOMEM;
	}
	eq->elevator_data = nd;

	INIT_LIST_HEAD(&nd->queue);

	spin_lock_irq(q->queue_lock);
	q->elevator = eq;
	spin_unlock_irq(q->queue_lock);
	return 0;
}

static void sstf_exit_queue(struct elevator_queue *e)
{
	struct sstf_data *nd = e->elevator_data;

	BUG_ON(!list_empty(&nd->queue));
	kfree(nd);
}

static struct elevator_type elevator_sstf = {
	.ops = {
		.elevator_merge_req_fn		= sstf_merged_requests,
		.elevator_dispatch_fn		= sstf_dispatch,
		.elevator_add_req_fn		= sstf_add_request,
		.elevator_former_req_fn		= sstf_former_request,
		.elevator_latter_req_fn		= sstf_latter_request,
		.elevator_init_fn		= sstf_init_queue,
		.elevator_exit_fn		= sstf_exit_queue,
	},
	.elevator_name = "sstf",
	.elevator_owner = THIS_MODULE,
};

static int __init sstf_init(void)
{
	return elv_register(&elevator_sstf);
}

static void __exit sstf_exit(void)
{
	elv_unregister(&elevator_sstf);
}

module_init(sstf_init);
module_exit(sstf_exit);


MODULE_AUTHOR("10-11");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("SSTF IO scheduler");
