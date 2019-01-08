#!/bin/bash
# 一次性处理git提交
#branch_name=`git symbolic-ref --short -q HEAD`
#branch_name=$(git symbolic-ref --short -q HEAD)
git add .
git commit -m "$1"
git push 
#git push origin "$branch_name"