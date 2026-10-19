#!/usr/bin/env python3
"""Generate the grouped runtime bar chart required by the report template."""
import csv
import os
from pathlib import Path
import tempfile

os.environ.setdefault('MPLCONFIGDIR', str(Path(tempfile.gettempdir()) / 'collatz-matplotlib'))
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

ROOT = Path(__file__).resolve().parents[1]
rows = list(csv.DictReader((ROOT / 'experiments/average_times.txt').open(), delimiter='\t'))
threads = [int(row['Threads']) for row in rows]
locked = [float(row['Locked_average_seconds']) for row in rows]
unlocked = [float(row['Unlocked_average_seconds']) for row in rows]
fig, ax = plt.subplots(figsize=(9, 6), layout='constrained')
ax.bar([t - .2 for t in threads], locked, width=.4, color='#2166ac', label='With locks')
ax.bar([t + .2 for t in threads], unlocked, width=.4, color='#c55b16', label='Without locks')
ax.set(title='Threads vs. average runtime', xlabel='Worker threads', ylabel='Average runtime (seconds)',
       xticks=threads, ylim=(0, 3.8))
ax.spines[['top', 'right']].set_visible(False)
ax.set_axisbelow(True)
ax.grid(axis='y', alpha=.25)
ax.legend(frameon=False)
for directory in ('experiments', 'deliverables'):
    fig.savefig(ROOT / directory / 'runtime_chart.png', dpi=300, facecolor='white')
plt.close(fig)
