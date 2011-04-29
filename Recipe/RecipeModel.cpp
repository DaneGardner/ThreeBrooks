/*!
   \file RecipeModel.cpp
   \author Dane Gardner <dane.gardner@gmail.com>
   \version 

   \section LICENSE
    This file is part of the ThreeBrooks homebrew recipe application
    Copyright (C) 2011 Dane Gardner

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

#include "RecipeModel.h"

RecipeModel::RecipeModel(QObject *parent) :
    QAbstractItemModel(parent)
{
}

QModelIndex RecipeModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row, column, parent))
        return QModelIndex();

    if(!parent.isValid()) {
        return createIndex(row, column, quint32(row));
    }

    return QModelIndex();
}

QModelIndex RecipeModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)
    return QModelIndex();
}

int RecipeModel::rowCount(const QModelIndex &parent) const
{
    if(!parent.isValid()) {
        return 5;
    }

    return 0;
}

int RecipeModel::columnCount(const QModelIndex &parent) const
{
    if(!parent.isValid()) {
        return 3;
    }

    return 0;
}

QVariant RecipeModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole && index.internalId() < 5) {
        if(index.column() == 0) {
            switch(index.row()) {
            case 0:
                return QVariant(QString().setNum(recipe()->originalGravity(), 'f', 3));
            case 1:
                return QVariant(QString().setNum(recipe()->finalGravity(), 'f', 3));
            case 2:
                return QVariant(QString().setNum(recipe()->bitterness(), 'f', 0));
            case 3:
                return QVariant(QString().setNum(recipe()->color(), 'f', 1));
            case 4:
                return QVariant(QString().setNum(recipe()->alcoholByVolume() * 100, 'f', 1));
            }
        } else {
            //TODO: Style high and low
            switch(index.row()) {
            case 0:
                if(index.column() == 1) return QVariant(QString().setNum(1.000, 'f', 3));
                if(index.column() == 2) return QVariant(QString().setNum(1.100, 'f', 3));
            case 1:
                if(index.column() == 1) return QVariant(QString().setNum(1.000, 'f', 3));
                if(index.column() == 2) return QVariant(QString().setNum(1.100, 'f', 3));
            case 2:
                if(index.column() == 1) return QVariant(QString().setNum(0.0, 'f', 0));
                if(index.column() == 2) return QVariant(QString().setNum(200.0, 'f', 0));
            case 3:
                if(index.column() == 1) return QVariant(QString().setNum(0.1, 'f', 1));
                if(index.column() == 2) return QVariant(QString().setNum(100.0, 'f', 1));
            case 4:
                if(index.column() == 1) return QVariant(QString().setNum(0.0, 'f', 1));
                if(index.column() == 2) return QVariant(QString().setNum(15.0, 'f', 1));
            }
        }
    }

    /* SRM to RGB came from here: http://methodbrewery.com/srm.php
       via: http://www.homebrewtalk.com/f84/ebc-srm-color-rgb-78018/ */
    if(role == Qt::DecorationRole && index.row() == 3) {
        int color = 0;

        switch(index.column()) {
        case 0: color = (int)(recipe()->color()*10); break;
        case 1: color = 1; break;
        case 2: color = 1000; break;
        }

        switch(color) {
        case 0:
        case 1: return QColor(250,250,210);
        case 2: return QColor(250,250,204);
        case 3: return QColor(250,250,199);
        case 4: return QColor(250,250,193);
        case 5: return QColor(250,250,188);
        case 6: return QColor(250,250,182);
        case 7: return QColor(250,250,177);
        case 8: return QColor(250,250,171);
        case 9: return QColor(250,250,166);
        case 10: return QColor(250,250,160);
        case 11: return QColor(250,250,155);
        case 12: return QColor(250,250,149);
        case 13: return QColor(250,250,144);
        case 14: return QColor(250,250,138);
        case 15: return QColor(250,250,133);
        case 16: return QColor(250,250,127);
        case 17: return QColor(250,250,122);
        case 18: return QColor(250,250,116);
        case 19: return QColor(250,250,111);
        case 20: return QColor(250,250,105);
        case 21: return QColor(250,250,100);
        case 22: return QColor(250,250,94);
        case 23: return QColor(250,250,89);
        case 24: return QColor(250,250,83);
        case 25: return QColor(250,250,78);
        case 26: return QColor(249,250,72);
        case 27: return QColor(248,249,67);
        case 28: return QColor(247,248,61);
        case 29: return QColor(246,247,56);
        case 30: return QColor(245,246,50);
        case 31: return QColor(244,245,45);
        case 32: return QColor(243,244,45);
        case 33: return QColor(242,242,45);
        case 34: return QColor(241,240,46);
        case 35: return QColor(240,238,46);
        case 36: return QColor(239,236,46);
        case 37: return QColor(238,234,46);
        case 38: return QColor(237,232,47);
        case 39: return QColor(236,230,47);
        case 40: return QColor(235,228,47);
        case 41: return QColor(234,226,47);
        case 42: return QColor(233,224,48);
        case 43: return QColor(232,222,48);
        case 44: return QColor(231,220,48);
        case 45: return QColor(230,218,48);
        case 46: return QColor(229,216,49);
        case 47: return QColor(228,214,49);
        case 48: return QColor(227,212,49);
        case 49: return QColor(226,210,49);
        case 50: return QColor(225,208,50);
        case 51: return QColor(224,206,50);
        case 52: return QColor(223,204,50);
        case 53: return QColor(222,202,50);
        case 54: return QColor(221,200,51);
        case 55: return QColor(220,198,51);
        case 56: return QColor(219,196,51);
        case 57: return QColor(218,194,51);
        case 58: return QColor(217,192,52);
        case 59: return QColor(216,190,52);
        case 60: return QColor(215,188,52);
        case 61: return QColor(214,186,52);
        case 62: return QColor(213,184,53);
        case 63: return QColor(212,182,53);
        case 64: return QColor(211,180,53);
        case 65: return QColor(210,178,53);
        case 66: return QColor(209,176,54);
        case 67: return QColor(208,174,54);
        case 68: return QColor(207,172,54);
        case 69: return QColor(206,170,54);
        case 70: return QColor(205,168,55);
        case 71: return QColor(204,166,55);
        case 72: return QColor(203,164,55);
        case 73: return QColor(202,162,55);
        case 74: return QColor(201,160,56);
        case 75: return QColor(200,158,56);
        case 76: return QColor(200,156,56);
        case 77: return QColor(199,154,56);
        case 78: return QColor(199,152,56);
        case 79: return QColor(198,150,56);
        case 80: return QColor(198,148,56);
        case 81: return QColor(197,146,56);
        case 82: return QColor(197,144,56);
        case 83: return QColor(196,142,56);
        case 84: return QColor(196,141,56);
        case 85: return QColor(195,140,56);
        case 86: return QColor(195,139,56);
        case 87: return QColor(194,139,56);
        case 88: return QColor(194,138,56);
        case 89: return QColor(193,137,56);
        case 90: return QColor(193,136,56);
        case 91: return QColor(192,136,56);
        case 92: return QColor(192,135,56);
        case 93: return QColor(192,134,56);
        case 94: return QColor(192,133,56);
        case 95: return QColor(192,133,56);
        case 96: return QColor(192,132,56);
        case 97: return QColor(192,131,56);
        case 98: return QColor(192,130,56);
        case 99: return QColor(192,130,56);
        case 100: return QColor(192,129,56);
        case 101: return QColor(192,128,56);
        case 102: return QColor(192,127,56);
        case 103: return QColor(192,127,56);
        case 104: return QColor(192,126,56);
        case 105: return QColor(192,125,56);
        case 106: return QColor(192,124,56);
        case 107: return QColor(192,124,56);
        case 108: return QColor(192,123,56);
        case 109: return QColor(192,122,56);
        case 110: return QColor(192,121,56);
        case 111: return QColor(192,121,56);
        case 112: return QColor(192,120,56);
        case 113: return QColor(192,119,56);
        case 114: return QColor(192,118,56);
        case 115: return QColor(192,118,56);
        case 116: return QColor(192,117,56);
        case 117: return QColor(192,116,56);
        case 118: return QColor(192,115,56);
        case 119: return QColor(192,115,56);
        case 120: return QColor(192,114,56);
        case 121: return QColor(192,113,56);
        case 122: return QColor(192,112,56);
        case 123: return QColor(192,112,56);
        case 124: return QColor(192,111,56);
        case 125: return QColor(192,110,56);
        case 126: return QColor(192,109,56);
        case 127: return QColor(192,109,56);
        case 128: return QColor(192,108,56);
        case 129: return QColor(191,107,56);
        case 130: return QColor(190,106,56);
        case 131: return QColor(189,106,56);
        case 132: return QColor(188,105,56);
        case 133: return QColor(187,104,56);
        case 134: return QColor(186,103,56);
        case 135: return QColor(185,103,56);
        case 136: return QColor(184,102,56);
        case 137: return QColor(183,101,56);
        case 138: return QColor(182,100,56);
        case 139: return QColor(181,100,56);
        case 140: return QColor(180,99,56);
        case 141: return QColor(179,98,56);
        case 142: return QColor(178,97,56);
        case 143: return QColor(177,97,56);
        case 144: return QColor(175,96,55);
        case 145: return QColor(174,95,55);
        case 146: return QColor(172,94,55);
        case 147: return QColor(171,94,55);
        case 148: return QColor(169,93,54);
        case 149: return QColor(168,92,54);
        case 150: return QColor(167,91,54);
        case 151: return QColor(165,91,54);
        case 152: return QColor(164,90,53);
        case 153: return QColor(162,89,53);
        case 154: return QColor(161,88,53);
        case 155: return QColor(159,88,53);
        case 156: return QColor(158,87,52);
        case 157: return QColor(157,86,52);
        case 158: return QColor(155,85,52);
        case 159: return QColor(154,85,52);
        case 160: return QColor(152,84,51);
        case 161: return QColor(151,83,51);
        case 162: return QColor(149,82,51);
        case 163: return QColor(148,82,51);
        case 164: return QColor(147,81,50);
        case 165: return QColor(145,80,50);
        case 166: return QColor(144,79,50);
        case 167: return QColor(142,78,50);
        case 168: return QColor(141,77,49);
        case 169: return QColor(139,76,49);
        case 170: return QColor(138,75,48);
        case 171: return QColor(137,75,47);
        case 172: return QColor(135,74,47);
        case 173: return QColor(134,73,46);
        case 174: return QColor(132,72,45);
        case 175: return QColor(131,72,45);
        case 176: return QColor(129,71,44);
        case 177: return QColor(128,70,43);
        case 178: return QColor(127,69,43);
        case 179: return QColor(125,69,42);
        case 180: return QColor(124,68,41);
        case 181: return QColor(122,67,41);
        case 182: return QColor(121,66,40);
        case 183: return QColor(119,66,39);
        case 184: return QColor(118,65,39);
        case 185: return QColor(117,64,38);
        case 186: return QColor(115,63,37);
        case 187: return QColor(114,63,37);
        case 188: return QColor(112,62,36);
        case 189: return QColor(111,61,35);
        case 190: return QColor(109,60,34);
        case 191: return QColor(108,60,33);
        case 192: return QColor(107,59,32);
        case 193: return QColor(105,58,31);
        case 194: return QColor(104,57,29);
        case 195: return QColor(102,57,28);
        case 196: return QColor(101,56,27);
        case 197: return QColor(99,55,26);
        case 198: return QColor(98,54,25);
        case 199: return QColor(97,54,24);
        case 200: return QColor(95,53,23);
        case 201: return QColor(94,52,21);
        case 202: return QColor(92,51,20);
        case 203: return QColor(91,51,19);
        case 204: return QColor(89,50,18);
        case 205: return QColor(88,49,17);
        case 206: return QColor(87,48,16);
        case 207: return QColor(85,48,15);
        case 208: return QColor(84,47,13);
        case 209: return QColor(82,46,12);
        case 210: return QColor(81,45,11);
        case 211: return QColor(79,45,10);
        case 212: return QColor(78,44,9);
        case 213: return QColor(77,43,8);
        case 214: return QColor(75,42,9);
        case 215: return QColor(74,42,9);
        case 216: return QColor(72,41,10);
        case 217: return QColor(71,40,10);
        case 218: return QColor(69,39,11);
        case 219: return QColor(68,39,11);
        case 220: return QColor(67,38,12);
        case 221: return QColor(65,37,12);
        case 222: return QColor(64,36,13);
        case 223: return QColor(62,36,13);
        case 224: return QColor(61,35,14);
        case 225: return QColor(59,34,14);
        case 226: return QColor(58,33,15);
        case 227: return QColor(57,33,15);
        case 228: return QColor(55,32,16);
        case 229: return QColor(54,31,16);
        case 230: return QColor(52,30,17);
        case 231: return QColor(51,30,17);
        case 232: return QColor(49,29,18);
        case 233: return QColor(48,28,18);
        case 234: return QColor(47,27,19);
        case 235: return QColor(45,27,19);
        case 236: return QColor(44,26,20);
        case 237: return QColor(42,25,20);
        case 238: return QColor(41,24,21);
        case 239: return QColor(39,24,21);
        case 240: return QColor(38,23,22);
        case 241: return QColor(37,22,21);
        case 242: return QColor(37,22,21);
        case 243: return QColor(36,22,21);
        case 244: return QColor(36,21,20);
        case 245: return QColor(35,21,20);
        case 246: return QColor(35,21,20);
        case 247: return QColor(34,20,19);
        case 248: return QColor(34,20,19);
        case 249: return QColor(33,20,19);
        case 250: return QColor(33,19,18);
        case 251: return QColor(32,19,18);
        case 252: return QColor(32,19,18);
        case 253: return QColor(31,18,17);
        case 254: return QColor(31,18,17);
        case 255: return QColor(30,18,17);
        case 256: return QColor(30,17,16);
        case 257: return QColor(29,17,16);
        case 258: return QColor(29,17,16);
        case 259: return QColor(28,16,15);
        case 260: return QColor(28,16,15);
        case 261: return QColor(27,16,15);
        case 262: return QColor(27,15,14);
        case 263: return QColor(26,15,14);
        case 264: return QColor(26,15,14);
        case 265: return QColor(25,14,13);
        case 266: return QColor(25,14,13);
        case 267: return QColor(24,14,13);
        case 268: return QColor(24,13,12);
        case 269: return QColor(23,13,12);
        case 270: return QColor(23,13,12);
        case 271: return QColor(22,12,11);
        case 272: return QColor(22,12,11);
        case 273: return QColor(21,12,11);
        case 274: return QColor(21,11,10);
        case 275: return QColor(20,11,10);
        case 276: return QColor(20,11,10);
        case 277: return QColor(19,10,9);
        case 278: return QColor(19,10,9);
        case 279: return QColor(18,10,9);
        case 280: return QColor(18,9,8);
        case 281: return QColor(17,9,8);
        case 282: return QColor(17,9,8);
        case 283: return QColor(16,8,7);
        case 284: return QColor(16,8,7);
        case 285: return QColor(15,8,7);
        case 286: return QColor(15,7,6);
        case 287: return QColor(14,7,6);
        case 288: return QColor(14,7,6);
        case 289: return QColor(13,6,5);
        case 290: return QColor(13,6,5);
        case 291: return QColor(12,6,5);
        case 292: return QColor(12,5,4);
        case 293: return QColor(11,5,4);
        case 294: return QColor(11,5,4);
        case 295: return QColor(10,4,3);
        case 296: return QColor(10,4,3);
        case 297: return QColor(9,4,3);
        case 298: return QColor(9,3,2);
        case 299: return QColor(8,3,2);
        case 300: return QColor(8,3,2);
        default: return QColor(6,3,2);
        }
    }

    return QVariant();
}

QVariant RecipeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Vertical && role == Qt::DisplayRole) {
        switch(section) {
        case 0:
            return QVariant(tr("Original Gravity (SG)"));
        case 1:
            return QVariant(tr("Final Gravity (SG)"));
        case 2:
            return QVariant(tr("Bitterness (IBU)"));
        case 3:
            return QVariant(tr("Color (SRM)"));
        case 4:
            return QVariant(tr("Alcohol by Volume"));
        }
    }

    if(orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch(section) {
        case 0:
            return QVariant(tr("Calculated"));
        case 1:
            return QVariant(tr("Style Low"));
        case 2:
            return QVariant(tr("Style High"));
        }
    }

    return QVariant();
}

void RecipeModel::recipeChanged()
{
    emit dataChanged(index(0,0), index(rowCount(),columnCount()));
}


