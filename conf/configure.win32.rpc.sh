#!/bin/sh
dir=$(dirname "$0")
$dir/fetch.sh
$dir/patch.sh
$dir/config.sh
