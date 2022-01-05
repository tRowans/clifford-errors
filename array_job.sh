#!/bin/bash
#SBATCH --account=def-raymond
#SBATCH --time=12:00:00
#SBATCH --mem=8G
#SBATCH --job-name=clifford-errors
#SBATCH --output=data/%x-%j.out
#SBATCH --mail-user=mvasmer@pitp.ca
#SBATCH --mail-type=ALL
#SBATCH --array=1-30

pwd 
echo "SLURM_JOB_ID=$SLURM_JOB_ID"
date

file=input/2022-01-05.csv
line=$(sed -n "1p" "$file")
echo $line
id=$SLURM_ARRAY_TASK_ID
id=$((id+1))
line=$(sed -n "${id}p" "$file")
echo $line
IFS=, read -ra params <<< "$line"

./build/src/czErrors ${params[0]} ${params[1]} ${params[2]} ${params[3]} ${params[4]} 0 0 ${params[5]} ${params[6]} ${params[7]}

date
