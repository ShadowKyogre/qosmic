/***************************************************************************
 *   Copyright (C) 2007, 2010 by David Bitseff                             *
 *   dbitsef@zipcon.net                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "genomecolorselector.h"
#include "logger.h"

GenomeColorSelector::GenomeColorSelector(QWidget* parent, GenomeVector* g)
	: QLabel(parent), genome(g), selected_y(0)
{
}

void GenomeColorSelector::mousePressEvent (QMouseEvent* e)
{
	if (e->button() == Qt::LeftButton)
	{
		last_y = selected_y;
		int y = 255 - e->pos().y();
		if (last_y != y)
			emit colorSelected(y / 255.);
	}
}

void GenomeColorSelector::mouseMoveEvent (QMouseEvent* e)
{
	if (e->buttons() & Qt::LeftButton && e->y() >= 0 && e->y() < 256)
	{
		int y = 255 - e->pos().y();
		emit colorSelected(y / 255.);
	}
}

void GenomeColorSelector::mouseReleaseEvent (QMouseEvent* e)
{
	if (e->button() == Qt::LeftButton)
	{
		if (last_y != selected_y)
			emit undoStateSignal();
	}
}

#define genome_ptr (genome->selectedGenome())

void GenomeColorSelector::setGenomeVector(GenomeVector* v)
{
	genome = v;
}

void GenomeColorSelector::repaintLabel()
{
	if (genome_ptr)
	{
		QSize s = maximumSize();
		int width  = s.width();
		int height = s.height();
		QImage palette(width, height, QImage::Format_RGB32);
		QPainter p(&palette);
		double color_hist[256];
		memset(color_hist, 0, sizeof(double)*256);
		if (flam3_colorhist(genome_ptr, 1, Util::get_isaac_randctx(), color_hist))
			logWarn(QString("GenomeColorSelector::repaintLabel : couldn't get flam3 color histogram"));

		QColor c;
		double rc, gc, bc;
		int chist_scale = ( width / 4 ) * width;
		for (int i = 0 ; i < 256 ; i++)
		{
			rc = genome_ptr->palette[i].color[0];
			gc = genome_ptr->palette[i].color[1];
			bc = genome_ptr->palette[i].color[2];
			if (rc < 0.0 || rc > 1.0 ||
				bc < 0.0 || bc > 1.0 ||
				gc < 0.0 || gc > 1.0)
				return;

			if (selected_y - 1 == i) // roundoff voodoo
			{
				c = QColor::fromRgbF(1.0 - rc, 1.0 - gc, 1.0 - bc);
				p.setPen(c);
				p.drawLine(0, 255 - i, width, 255 - i);
			}
			else
			{
				c = QColor::fromRgbF(rc, gc, bc);
				p.setPen(c);
				p.drawLine(0, 255 - i, width, 255 - i);
				p.setPen(c.darker(300));
				p.drawLine(0, 255 - i, (width / 2) , 255 - i);
				p.setPen(QColor::fromRgb(255,255,255,192));
				p.drawLine(0, 255 - i, chist_scale * color_hist[i], 255 - i);
			}
		}
		setPixmap(QPixmap::fromImage( palette ));
	}
}

void GenomeColorSelector::setSelectedIndex(int y)
{
	int this_y = 1 + y;
	selected_y = this_y; // voodoo too
	repaintLabel();
}

int GenomeColorSelector::selectedIndex()
{
	return selected_y;
}