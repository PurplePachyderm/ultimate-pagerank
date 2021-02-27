#!/bin/bash

# Downloads an example edge list from :
# http://snap.stanford.edu/data/ego-Facebook.html

wget http://snap.stanford.edu/data/facebook_combined.txt.gz
gunzip facebook_combined.txt.gz

# We must add the number of nodes
sed -i '1s/^/4039\n/' facebook_combined.txt
