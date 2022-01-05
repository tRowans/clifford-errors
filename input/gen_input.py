import csv
import subprocess
import numpy as np
from datetime import datetime

Lmin = 6
Lmax = 12
Ls = range(Lmin, Lmax + 1, 2)
ps = np.linspace(0.005, 0.014, 10)
runs = int(1e4)
job = 0
git_hash = subprocess.check_output(['git', 'rev-parse',  'HEAD']).strip()
link = 1
today = datetime.today().strftime('%Y-%m-%d')
max_iter = 10
osd_order = 6
osd_method = 'osd_cs'

with open(f'{today}.csv', 'w') as csv_file:
    writer = csv.writer(csv_file, delimiter=',')
    writer.writerow(['L', 'p', 'q', 'runs', 'linking', 'max_iter', 'osd_order', 'osd_method', 'job_number', 'git hash (metadata)'])
    for L in Ls:
        for p in ps:
            writer.writerow([L, p, p, runs, link, max_iter, osd_order, osd_method, job, git_hash])
            job += 1
