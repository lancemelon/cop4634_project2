#!/usr/bin/env python3
"""Add required native Excel charts, preserving the supplied workbook's cells.

Requires openpyxl. Re-running replaces the charts this script manages.
"""
from pathlib import Path
from openpyxl import load_workbook
from openpyxl.chart import BarChart, Reference

ROOT = Path(__file__).resolve().parents[1]
path = ROOT / 'deliverables' / 'cop4634 proj2.xlsx'
workbook = load_workbook(path)
ws = workbook['runtimes']
ws._charts.clear()
chart = BarChart()
chart.type = 'col'
chart.grouping = 'clustered'
chart.overlap = 0
chart.title = 'Threads vs. average runtime (N = 3,000,000; 10 runs)'
chart.x_axis.title = 'Worker threads'
chart.y_axis.title = 'Average runtime (seconds)'
chart.y_axis.scaling.min = 0
chart.width, chart.height = 25, 15
chart.add_data(Reference(ws, min_col=2, max_col=3, min_row=1, max_row=9), titles_from_data=True)
chart.set_categories(Reference(ws, min_col=1, min_row=2, max_row=9))
for series, color in zip(chart.series, ('2166AC', 'C55B16')):
    series.graphicalProperties.solidFill = color
    series.graphicalProperties.line.noFill = True
ws.add_chart(chart, 'E2')
ws['E32'] = 'Unlocked T > 1 histograms are incorrect. Locked T=4 mean retains the 14.682816 s observation.'
ws.freeze_panes = 'A2'
for col, width in (('A', 12), ('B', 29), ('C', 30)):
    ws.column_dimensions[col].width = width
for row in ws.iter_rows(min_row=2, max_row=9, min_col=2, max_col=3):
    for cell in row:
        cell.number_format = '0.000000000'
ws = workbook['histogram']
ws._charts.clear()
chart = BarChart()
chart.title = 'Collatz stopping-time histogram (N = 3,000,000)'
chart.x_axis.title = 'Stopping time (steps to reach 1)'
chart.y_axis.title = 'Frequency'
chart.y_axis.scaling.min = 0
chart.width, chart.height = 29, 16
chart.gapWidth = 0
chart.legend = None
# Display buckets 0..559; all 1001 buckets remain in the worksheet.
chart.add_data(Reference(ws, min_col=2, min_row=1, max_row=561), titles_from_data=True)
chart.set_categories(Reference(ws, min_col=1, min_row=2, max_row=561))
chart.x_axis.tickLblSkip = 50
chart.series[0].graphicalProperties.solidFill = '2166AC'
chart.series[0].graphicalProperties.line.noFill = True
ws.add_chart(chart, 'D2')
ws['D34'] = 'Buckets 560 through 1000 are zero; the complete histogram remains in columns A:B.'
ws.freeze_panes = 'A2'
ws.column_dimensions['A'].width = 19
ws.column_dimensions['B'].width = 16
workbook.save(path)
print(path)
