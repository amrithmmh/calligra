/* This file is part of the KDE project
* Copyright (c) 2010 Jan Hambrecht <jaham@gmx.net>
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Library General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this library; see the file COPYING.LIB.  If not, write to
* the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
* Boston, MA 02110-1301, USA.
*/

#ifndef COLORCHANNELCONVERSION_H
#define COLORCHANNELCONVERSION_H

/**
* Lookup table to convert color value from int [0..255] to double [0..1]
*/
static const qreal fromIntColor[256] = {
    0.00000000000000000, 0.00392156862745098, 0.007843137254901961, 0.01176470588235294, 0.01568627450980392, 0.0196078431372549, 0.02352941176470588, 0.02745098039215686, 0.03137254901960784,
    0.03529411764705882, 0.0392156862745098, 0.04313725490196078, 0.04705882352941176, 0.05098039215686274, 0.05490196078431372, 0.05882352941176471, 0.06274509803921569,
    0.06666666666666667, 0.07058823529411765, 0.07450980392156863, 0.07843137254901961, 0.08235294117647059, 0.08627450980392157, 0.09019607843137255, 0.09411764705882353,
    0.09803921568627451, 0.1019607843137255, 0.1058823529411765, 0.1098039215686274, 0.1137254901960784, 0.1176470588235294, 0.1215686274509804, 0.1254901960784314,
    0.1294117647058824, 0.1333333333333333, 0.1372549019607843, 0.1411764705882353, 0.1450980392156863, 0.1490196078431373, 0.1529411764705882, 0.1568627450980392,
    0.1607843137254902, 0.1647058823529412, 0.1686274509803922, 0.1725490196078431, 0.1764705882352941, 0.1803921568627451, 0.1843137254901961, 0.1882352941176471,
    0.1921568627450981, 0.196078431372549, 0.2, 0.203921568627451, 0.207843137254902, 0.2117647058823529, 0.2156862745098039, 0.2196078431372549,
    0.2235294117647059, 0.2274509803921569, 0.2313725490196079, 0.2352941176470588, 0.2392156862745098, 0.2431372549019608, 0.2470588235294118, 0.2509803921568627,
    0.2549019607843137, 0.2588235294117647, 0.2627450980392157, 0.2666666666666667, 0.2705882352941176, 0.2745098039215687, 0.2784313725490196, 0.2823529411764706,
    0.2862745098039216, 0.2901960784313726, 0.2941176470588235, 0.2980392156862745, 0.3019607843137255, 0.3058823529411765, 0.3098039215686275, 0.3137254901960784,
    0.3176470588235294, 0.3215686274509804, 0.3254901960784314, 0.3294117647058823, 0.3333333333333333, 0.3372549019607843, 0.3411764705882353, 0.3450980392156863,
    0.3490196078431372, 0.3529411764705883, 0.3568627450980392, 0.3607843137254902, 0.3647058823529412, 0.3686274509803922, 0.3725490196078431, 0.3764705882352941,
    0.3803921568627451, 0.3843137254901961, 0.3882352941176471, 0.392156862745098, 0.396078431372549, 0.4, 0.403921568627451, 0.407843137254902,
    0.4117647058823529, 0.4156862745098039, 0.4196078431372549, 0.4235294117647059, 0.4274509803921568, 0.4313725490196079, 0.4352941176470588, 0.4392156862745098,
    0.4431372549019608, 0.4470588235294118, 0.4509803921568628, 0.4549019607843137, 0.4588235294117647, 0.4627450980392157, 0.4666666666666667, 0.4705882352941176,
    0.4745098039215686, 0.4784313725490196, 0.4823529411764706, 0.4862745098039216, 0.4901960784313725, 0.4941176470588236, 0.4980392156862745, 0.5019607843137255,
    0.5058823529411764, 0.5098039215686274, 0.5137254901960784, 0.5176470588235295, 0.5215686274509804, 0.5254901960784314, 0.5294117647058824, 0.5333333333333333,
    0.5372549019607843, 0.5411764705882353, 0.5450980392156862, 0.5490196078431373, 0.5529411764705883, 0.5568627450980392, 0.5607843137254902, 0.5647058823529412,
    0.5686274509803921, 0.5725490196078431, 0.5764705882352941, 0.5803921568627451, 0.5843137254901961, 0.5882352941176471, 0.592156862745098, 0.596078431372549,
    0.6000000000000000, 0.6039215686274509, 0.6078431372549019, 0.611764705882353, 0.615686274509804, 0.6196078431372549, 0.6235294117647059, 0.6274509803921569,
    0.6313725490196078, 0.6352941176470588, 0.6392156862745098, 0.6431372549019608, 0.6470588235294118, 0.6509803921568628, 0.6549019607843137, 0.6588235294117647,
    0.6627450980392157, 0.6666666666666666, 0.6705882352941176, 0.6745098039215687, 0.6784313725490196, 0.6823529411764706, 0.6862745098039216, 0.6901960784313725,
    0.6941176470588235, 0.6980392156862745, 0.7019607843137254, 0.7058823529411765, 0.7098039215686275, 0.7137254901960784, 0.7176470588235294, 0.7215686274509804,
    0.7254901960784313, 0.7294117647058823, 0.7333333333333333, 0.7372549019607844, 0.7411764705882353, 0.7450980392156863, 0.7490196078431373, 0.7529411764705882,
    0.7568627450980392, 0.7607843137254902, 0.7647058823529411, 0.7686274509803922, 0.7725490196078432, 0.7764705882352941, 0.7803921568627451, 0.7843137254901961,
    0.788235294117647, 0.792156862745098, 0.796078431372549, 0.8, 0.803921568627451, 0.807843137254902, 0.8117647058823529, 0.8156862745098039,
    0.8196078431372549, 0.8235294117647058, 0.8274509803921568, 0.8313725490196079, 0.8352941176470589, 0.8392156862745098, 0.8431372549019608, 0.8470588235294118,
    0.8509803921568627, 0.8549019607843137, 0.8588235294117647, 0.8627450980392157, 0.8666666666666667, 0.8705882352941177, 0.8745098039215686, 0.8784313725490196,
    0.8823529411764706, 0.8862745098039215, 0.8901960784313725, 0.8941176470588236, 0.8980392156862745, 0.9019607843137255, 0.9058823529411765, 0.9098039215686274,
    0.9137254901960784, 0.9176470588235294, 0.9215686274509803, 0.9254901960784314, 0.9294117647058824, 0.9333333333333333, 0.9372549019607843, 0.9411764705882353,
    0.9450980392156862, 0.9490196078431372, 0.9529411764705882, 0.9568627450980393, 0.9607843137254902, 0.9647058823529412, 0.9686274509803922, 0.9725490196078431,
    0.9764705882352941, 0.9803921568627451, 0.984313725490196, 0.9882352941176471, 0.9921568627450981, 0.996078431372549, 1
};

#endif // COLORCHANNELCONVERSION_H
