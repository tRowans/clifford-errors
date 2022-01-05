#!/bin/bash
#SBATCH --account=def-raymond
<<<<<<< HEAD
#SBATCH --time=4:00:00
#SBATCH --mem=4G
#SBATCH --job-name=clifford-errors-L6
#SBATCH --output=data/%x-%j.out
#SBATCH --mail-user=mvasmer@pitp.ca
#SBATCH --mail-type=ALL
#SBATCH --array=1-10
=======
#SBATCH --time=12:00:00
#SBATCH --mem=8G
#SBATCH --job-name=clifford-errors
#SBATCH --output=data/%x-%j.out
#SBATCH --mail-user=mvasmer@pitp.ca
#SBATCH --mail-type=ALL
#SBATCH --array=1-30
>>>>>>> Script and input for compute canada

pwd 
echo "SLURM_JOB_ID=$SLURM_JOB_ID"
date

<<<<<<< HEAD
file=/home/mvasmer/src/clifford-errors/input/2022-01-05.csv
=======
file=input/2022-01-05.csv
>>>>>>> Script and input for compute canada
line=$(sed -n "1p" "$file")
echo $line
id=$SLURM_ARRAY_TASK_ID
id=$((id+1))
line=$(sed -n "${id}p" "$file")
echo $line
IFS=, read -ra params <<< "$line"

./build/src/czErrors ${params[0]} ${params[1]} ${params[2]} ${params[3]} ${params[4]} 0 0 ${params[5]} ${params[6]} ${params[7]}

<<<<<<< HEAD
date
=======
date
>>>>>>> Script and input for compute canada
