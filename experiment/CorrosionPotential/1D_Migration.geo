// Gmsh project created on Mon Dec 21 13:02:03 2020
SetFactory("OpenCASCADE");
//+
Point(1) = {0, 0, 0, 1.0};
//+
Point(2) = {12.7324, 0, 0, 1.0};
//+
Line(1) = {1,2};
//+
Transfinite Curve {1} = 3000 Using Progression 1.005;
//+
Physical Curve("Solution") = {1};//+
Physical Point("Copper_top") = {1};
