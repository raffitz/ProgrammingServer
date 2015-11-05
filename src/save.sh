#!/bin/sh

rm ./*~
tar -vcf ../psis-$(date +%F-%Hh%M).tar ../projecto/*
