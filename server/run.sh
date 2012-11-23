#!/bin/bash
#
# Export the required IOs to userland here
#
#gpio export 18 out

# Invoke exec in userland
sudo ./main 1234

# Remove exports
#gpio unexportall
