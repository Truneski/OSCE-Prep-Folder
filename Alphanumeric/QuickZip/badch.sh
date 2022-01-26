#!/bin/bash
 for i in {0..255}; do printf "\\\x%02x" $i;done
