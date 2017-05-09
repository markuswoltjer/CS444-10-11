from random import shuffle
from glob import glob

files = glob("/*")
shuffle(files)
print >> open('gensectors_out.txt', 'w'), files
