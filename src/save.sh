#!/bin/sh

rm ./*~
tar -vczf ../psis-$(date +%F-%Hh%M).tgz ../projecto/*
