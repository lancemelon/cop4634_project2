#!/usr/bin/env python3
"""Build report.pdf from report.md. Requires reportlab (pip install reportlab)."""
from pathlib import Path
import re
from xml.sax.saxutils import escape

from reportlab.lib import colors
from reportlab.lib.enums import TA_CENTER
from reportlab.lib.styles import getSampleStyleSheet, ParagraphStyle
from reportlab.lib.utils import ImageReader
from reportlab.platypus import SimpleDocTemplate, Paragraph, Spacer, Image, Table, TableStyle, PageBreak

ROOT = Path(__file__).resolve().parents[1]
styles = getSampleStyleSheet()
styles['Normal'].fontName = 'Times-Roman'
styles['Normal'].fontSize = 11
styles['Normal'].leading = 14
styles['Normal'].spaceAfter = 9
styles['Heading1'].fontSize = 19
styles['Heading1'].leading = 23
styles['Heading1'].textColor = colors.HexColor('#17324d')
styles['Heading2'].fontSize = 14
styles['Heading2'].leading = 18
styles['Heading2'].textColor = colors.HexColor('#17324d')
styles['Heading3'].fontSize = 12
styles['Heading3'].leading = 15
styles.add(ParagraphStyle('CaptionSmall', parent=styles['Normal'], fontName='Times-Italic', fontSize=9, leading=11))
styles.add(ParagraphStyle('CellSmall', parent=styles['Normal'], fontSize=9, leading=11, spaceAfter=0))


def inline(text):
    text = escape(text)
    return re.sub(r'\*\*(.+?)\*\*', r'<b>\1</b>', text)


def footer(canvas, doc):
    canvas.saveState()
    canvas.setStrokeColor(colors.HexColor('#c7d1db'))
    canvas.line(48, 37, 564, 37)
    canvas.setFont('Helvetica', 8)
    canvas.setFillColor(colors.HexColor('#52606d'))
    canvas.drawString(48, 25, 'COP4634 | Project 2 | Draft')
    canvas.drawRightString(564, 25, str(doc.page))
    canvas.restoreState()


def build():
    blocks = re.split(r'\n\s*\n', (ROOT / 'report.md').read_text().strip())
    story = []
    for block in blocks:
        block = block.strip()
        if block == '---':
            story.append(PageBreak())
        elif block.startswith('# '):
            story.append(Paragraph(inline(block[2:]), styles['Heading1']))
        elif block.startswith('## '):
            story.append(Paragraph(inline(block[3:]), styles['Heading2']))
        elif block.startswith('### '):
            story.append(Paragraph(inline(block[4:]), styles['Heading3']))
        elif block.startswith('!['):
            match = re.fullmatch(r'!\[(.*?)\]\((.*?)\)', block)
            path = ROOT / match[2]
            width, height = ImageReader(str(path)).getSize()
            target_width = 400 if path.name == 'runtime_boxplot.png' else 475
            story.extend([Image(str(path), width=target_width, height=target_width * height / width), Spacer(1, 7)])
        elif block.startswith('|'):
            rows = [[cell.strip() for cell in row.strip('|').split('|')] for row in block.splitlines()]
            rows = [row for row in rows if not all(re.fullmatch(r'[:\- ]+', cell) for cell in row)]
            table = Table([[Paragraph(inline(cell), styles['CellSmall']) for cell in row] for row in rows],
                          colWidths=[78, 132, 140, 125], hAlign='CENTER', repeatRows=1)
            table.setStyle(TableStyle([
                ('BACKGROUND', (0, 0), (-1, 0), colors.HexColor('#e5edf5')),
                ('ROWBACKGROUNDS', (0, 1), (-1, -1), [colors.white, colors.HexColor('#f4f6f8')]),
                ('LINEBELOW', (0, 0), (-1, 0), 0.6, colors.HexColor('#9eafbf')),
                ('TOPPADDING', (0, 0), (-1, -1), 5), ('BOTTOMPADDING', (0, 0), (-1, -1), 5),
            ]))
            story.extend([table, Spacer(1, 10)])
        else:
            style = styles['CaptionSmall'] if block.startswith('Figure ') else styles['Normal']
            story.append(Paragraph(inline(' '.join(block.splitlines())), style))
    doc = SimpleDocTemplate(str(ROOT / 'report.pdf'), pagesize=(612, 792),
                            rightMargin=48, leftMargin=48, topMargin=42, bottomMargin=50,
                            title='Project 2 Midpoint Report (Draft)',
                            author='Lance Penaflor')
    doc.build(story, onFirstPage=footer, onLaterPages=footer)
    print(ROOT / 'report.pdf')


if __name__ == '__main__':
    build()
