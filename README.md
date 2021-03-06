ThreeBrooks homebrew recipe application
=======================================

SUMMARY
-------
ThreeBrooks is a dead-simple, cross-platform homebrewing recipe application.  I grew tired of the "kitchen sink" applications that tried to squeeze every single feature into a single panel.  Database programmers should not be GUI designers, otherwise you end up with something that looks like a Microsoft Access application.

As a matter of fact, I was so unhappy with most of the desktop software available for homebrewing that I was using spreadsheets to do most of my calculations.  While doing this, I realized that every application out there was shooting mice with cannons.  I don't need all of that functionality, and prefer a simplified interface.

Given that there weren't any real open-source  applications offering all of the features that I wanted to see, and none that I didn't, I decided to write my own from scratch.  It also allowed me to learn a bit more of the details about GUI programming with the Qt4 framework, which I use for my work.


FEATURES
--------
Officially works with both Qt4 and Qt5.  Qt 4.6.0 and higher to be exact.

**Ingredient Filtering** - the biggest feature that I wanted to see in my application was a method for searching the ingredient list.  All too often it would take me several minutes to find an ingredient in an alphabetized list, which started with the maltster's name for some reason.  More than once, I would add the ingredient only to find that it was already there, but began with a different name than expected!  No more.  With the filtering feature in this application, I can quickly find any ingredient without any fuss (beyond having to write my own application to get this feature).

**In-Situ Editing** - I hate popups.  I prefer to double-click on something, and edit it where it is.  I've created popups in some areas of this application because they are easy to create, but as development progresses, I will slowly remove them for more streamlined alternatives.  

**XML** - everything is stored and retrieved using XML.  This means that ingredient files are hand- editable.  It also means that they can be imported by other applications very easily.

**Drag and Drop** - the de-facto applications in this field do not do drag and drop.  This is horribly unnatural feeling, and makes life more difficult than it should be.  Every ingredient in this application can be moved around in an intuitive way.

**Portable** - this application attempts to store all of its files in easily accesible locations.  On Windows machines, it stores nothing in the Windows Registry.


EQUATIONS
---------
Debates have raged over which equation to use for calculating the bitterness that hops add to a beer throughout the boil time.  I have personally reviewed the three (Rager, Garetz and Tinseth) and personally use a modified Tinseth equation.  Modified in that I use it properly, whereas other applications tend to simplify it, not using it as intended.

Tinseth's method can account for hop utilization based on the wort gravity that the hops will experience over each's individual boil.  Rager and Garetz assume that all of the hops additions will experience a linear extraction based on the final gravity and the time that they are boiled.  Corrective adjustments have to be made for the latter two formulae in high gravity boils.

At any rate, it really comes down to picking one equation and sticking with it.  If all of your recipes come up with a number on a single scale, comparisons can be made, making adjustments easier.  For example, it doesn't matter if you use Celcius or Fahrenheit, as long as you don't try to compare the two --100�C is not the same as 100�F-- but they both effectively describe the same phenomenon.

Here are some links for those that would like to read more on the various aspects of each method.

 * http://users.rcn.com/thor.dnai/dboard/dbnewsl/t9510d.htm
 * See Kevin Masaryk's comment toward the middle of this page: http://club.azhomebrewers.org/forum/topics/hop-formula-calculation-survey
 * http://www.realbeer.com/hops/
 * http://www.backyardbrewers.com/?p=39


LICENSE
-------
These files are part of the ThreeBrooks homebrew recipe application Copyright (C) 2011-2013 Dane Gardner

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.
