/*!
   \file main.cpp
   \author Dane Gardner <dane.gardner@gmail.com>
   \version 

   \section LICENSE
    This file is part of the ThreeBrooks homebrew recipe application
    Copyright (C) 2011-2013 Dane Gardner

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

   \section DESCRIPTION

 */

#include <QApplication>
#include <QCoreApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Dane Gardner");
    QCoreApplication::setOrganizationDomain("threebrooks.com");
    QCoreApplication::setApplicationName("Three Brooks");
    QCoreApplication::setApplicationVersion("0.1 Beta 1");

    QApplication::instance()->setProperty("helpUrl", "https://github.com/DaneGardner/ThreeBrooks/wiki");

    QApplication::instance()->setProperty("Copyright", "Copyright © 2011-2013 Dane Gardner. All rights reserved.");

    QApplication::instance()->setProperty("License",
                       "This program is free software: you can redistribute it and/or modify "
                       "it under the terms of the GNU General Public License as published by "
                       "the Free Software Foundation, either version 3 of the License, or "
                       "(at your option) any later version.<br/>"
                       "<br/>"
                       "This program is distributed in the hope that it will be useful, "
                       "but WITHOUT ANY WARRANTY; without even the implied warranty of "
                       "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
                       "GNU General Public License for more details.<br/>"
                       "<br/>"
                       "You should have received a copy of the GNU General Public License "
                       "along with this program.  If not, see &lt;http://www.gnu.org/licenses/&gt;.");

    MainWindow *w = MainWindow::instance();
    w->show();
    return a.exec();
}
