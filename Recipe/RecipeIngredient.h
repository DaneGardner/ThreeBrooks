#ifndef RECIPEINGREDIENT_H
#define RECIPEINGREDIENT_H

#include <QObject>
#include <QDomDocument>
#include "../Ingredient/Ingredient.h"
#include "../Quantity.h"

class RecipeIngredient : public QObject
{
    Q_OBJECT
public:
    explicit RecipeIngredient(Ingredient *ingredient, QObject *parent = 0);
    explicit RecipeIngredient(Ingredient *ingredient, Quantity quantity = Quantity(),
                              QObject *parent = 0);
    explicit RecipeIngredient(Ingredient *ingredient, Quantity quantity = Quantity(),
                              double minutes = 0, QObject *parent = 0);
    explicit RecipeIngredient(QDomElement element, QObject *parent = 0);

    virtual Ingredient *ingredient();

    virtual QString name() const;
    virtual QString type() const;

    virtual Quantity quantity() const;
    virtual void setQuantity(Quantity quantity);

    virtual double minutes() const;
    virtual void setMinutes(double minutes);

    QDomElement toXml(QDomDocument document);
    void fromXml(QDomElement element);

signals:
    void dataChanged();

protected:
    virtual void initialize();
    virtual void setIngredient(Ingredient* ingredient);

    Ingredient *_ingredient;
    Quantity _quantity;
    double _minutes;

protected slots:
    void ingredientChanged();
};

#endif // RECIPEINGREDIENT_H
