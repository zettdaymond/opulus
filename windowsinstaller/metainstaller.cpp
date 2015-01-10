/***************************************************************************
 *   Copyright (C) 2007 by Hugo Parente Lima <hugo.pl@gmail.com>           *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 ***************************************************************************/

#include <QFile>
#include <QTextStream>
#include "version.h"
using namespace std;

int main(int argc, const char** argv) {
	QTextStream out(stderr);

	if (argc < 3) {
		out << "need more args =]\n";
		return 1;
	}
	out << argv[1];
	const char* SOURCE_DIR = argv[1];
	const char* BUILD_DIR = argv[2];
	const char* QT_DIR = argv[3];
	
	QString scriptName(SOURCE_DIR);
	scriptName += "/template.iss"; 
	QFile scriptFile(scriptName);
	if (!scriptFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		out << "Template file not found: " << scriptName << endl;
		return 2;
	}
	Q_ASSERT(scriptFile.exists());
	QString script(scriptFile.readAll());
	
	script.replace("##VERSION##", opulusVersion());
	script.replace("##SOURCE_DIR##", QString(SOURCE_DIR));
	script.replace("##BUILD_DIR##", QString(BUILD_DIR));
	script.replace("##QT_DIR##", QString(QT_DIR));
	
	QFile issScript("opulus.iss");
	if (!issScript.open(QIODevice::WriteOnly | QIODevice::Text)) {
		out << "Error saving script\n";
		return 3;
	}
	issScript.write(script.toUtf8());
	issScript.close();
	
	return system("iscc opulus.iss");
}
