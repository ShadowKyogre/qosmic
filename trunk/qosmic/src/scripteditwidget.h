/***************************************************************************
 *   Copyright (C) 2007 - 2011 by David Bitseff                            *
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
#ifndef SCRIPTEDITWIDGET_H
#define SCRIPTEDITWIDGET_H

#include "ui_scripteditwidget.h"
#include "lua/luathread.h"

class MainWindow;

class ScriptEditWidget : public QWidget, private Ui::ScriptEditWidget
{
	Q_OBJECT

	QString script_filename;
	Lua::LuaThread lua_thread;
	MainWindow* mw;

	public:
		ScriptEditWidget(MainWindow* m, QWidget *parent = 0);
		~ScriptEditWidget();
		bool isScriptRunning() const;

	public slots:
		void startScript();
		void stopScript();
		void scriptFinishedAction();
		void scriptLoadedAction();
		void scriptSavedAction();
		void loadScript(const QString&);

	protected slots:
		void runButtonPressedAction();
		void updateCursorLabel();
		void appendScriptOutput(const QString&);
		void configPressedAction();

	protected:
		void closeEvent(QCloseEvent*);
};


#include "ui_scripteditconfigdialog.h"

class ScriptEditConfigDialog : public QDialog, private Ui::ScriptEditConfigDialog
{
	Q_OBJECT

	QFontDatabase fonts;
	QString cur_size;

	public:
		explicit ScriptEditConfigDialog(QWidget* =0);
		void setFont(const QFont&);
		QFont getFont() const;
		void setLuaEnvText(const QString&);
		QString getLuaEnvText() const;

	protected slots:
		void fontSelectBoxChanged(const QFont&);

	protected:
		void changeEvent(QEvent*);
};


#endif
