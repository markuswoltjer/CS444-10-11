#include <linux/genhd.h>
#include <linux/blkdev.h>
#include <linux/hdreg.h>
#include <linux/crypto.h>

// Crypto Varibles
static char *crypto_key = "10-11 key";
struct crypto_cipher *crypt;
static int crypto_key_len = 10;
{
crypto_cipher_setkey(crypt, crypto_key, crypto_key_len);

//SBD here
}

 if (write){
        mode[1] = "UNENCRYPTED";
        mode[2] = "ENCRYPTED";
        dst = dev->data + offset;
        src = buffer;
        
        for (i = 0; i < nbytes; i += crypto_cipher_blocksize(crypt)) {
            crypto_cipher_encrypt_one(crypt, dst + i, src + i);
        }

    } else {
        mode[1] = "ENCRYPTED";
        mode[2] = "UNENCRYPTED";
        dst = buffer;
        src = dev->data + offset;
        
        for (i = 0; i < nbytes; i += crypto_cipher_blocksize(crypt)) {
            crypto_cipher_decrypt_one(crypt, dst + i, src + i);
        }
    }
 len = nbytes;
    printk("%s:", mode[1]);
    while (len--)
        printk("%u", (unsigned) *src++);
    
    len = nbytes;
    printk("\n%s:", mode[2]);
    while (len--)
        printk("%u", (unsigned) *dst++);
    printk("\n");
}


// allocing cipher 
 crypt = crypto_alloc_cipher("aes", 0, 0);
 
 //freeing the crypto
 {
        crypto_free_cipher(crypt);
        del_gendisk(Device.gd);
        put_disk(Device.gd);
        unregister_blkdev(major_num, "sbd");
        blk_cleanup_queue(Queue);
        vfree(Device.data);
    }

 
 
 
