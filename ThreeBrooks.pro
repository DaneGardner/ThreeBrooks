#
#    This file is part of the ThreeBrooks homebrew recipe application
#    Copyright (C) 2011 Dane Gardner
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

QT          += core gui xml

TARGET       = ThreeBrooks
TEMPLATE     = app

SOURCES     += main.cpp\
               MainWindow.cpp \
               Recipe/Recipe.cpp \
               Ingredient/Ingredient.cpp \
               Ingredient/GrainIngredient.cpp \
               Ingredient/HopsIngredient.cpp \
               Ingredient/YeastIngredient.cpp \
               Recipe/RecipeIngredient.cpp \
               Recipe/RecipeIngredientModel.cpp \
               Ingredient/IngredientModel.cpp \
               Quantity.cpp \
               Recipe/RecipeWidget.cpp \
               Recipe/RecipeIngredientDelegate.cpp \
               Recipe/RecipeModel.cpp \
               Ingredient/IngredientToolbox.cpp \
               Ingredient/IngredientEditDialog.cpp \
               NotificationBar.cpp \
               PrettyTabWidget.cpp

HEADERS     += MainWindow.h \
               Recipe/Recipe.h \
               Ingredient/Ingredient.h \
               Ingredient/GrainIngredient.h \
               Ingredient/HopsIngredient.h \
               Ingredient/YeastIngredient.h \
               Recipe/RecipeIngredient.h \
               Recipe/RecipeIngredientModel.h \
               Ingredient/IngredientModel.h \
               Quantity.h \
               Recipe/RecipeWidget.h \
               Recipe/RecipeIngredientDelegate.h \
               Recipe/RecipeModel.h \
               Ingredient/IngredientToolbox.h \
               Ingredient/IngredientEditDialog.h \
               NotificationBar.h \
               PrettyTabWidget.h

FORMS       += MainWindow.ui \
               Recipe/RecipeWidget.ui \
               Ingredient/IngredientToolbox.ui \
               Ingredient/IngredientEditDialog.ui \
               NotificationBar.ui

RESOURCES   += Resources.qrc

OTHER_FILES += stylesheet.css \
               ingredients.xml \
               settings.ini \
               ThreeBrooks.rc \
               README \
               LICENSE

win32: RC_FILE               = ThreeBrooks.rc
macx: RC_FILE                = icons/hop-cone-3.icns

target.path                  = /
INSTALLS                    += target

ingredients.path             = /
ingredients.files            = ingredients.xml
INSTALLS                    += ingredients

stylesheet.path              = /
stylesheet.files             = stylesheet.css
INSTALLS                    += stylesheet
