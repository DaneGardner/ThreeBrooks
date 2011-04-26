#-------------------------------------------------
#
# Project created by QtCreator 2011-03-21T15:01:51
#
#-------------------------------------------------

QT       += core gui xml

TARGET = ThreeBrooks
TEMPLATE = app


SOURCES  += main.cpp\
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
            Ingredient/IngredientEditDialog.cpp

HEADERS  += MainWindow.h \
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
            Ingredient/IngredientEditDialog.h

FORMS    += MainWindow.ui \
            Recipe/RecipeWidget.ui \
            Ingredient/IngredientToolbox.ui \
            Ingredient/IngredientEditDialog.ui

RESOURCES += Resources.qrc

OTHER_FILES += \
    stylesheet.css \
    ingredients.xml
