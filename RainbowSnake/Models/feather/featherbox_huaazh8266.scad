printBase = false;
printVertWing = true;
printButtonWing = false;


// Base
if (printBase)
difference(){
  cube([30, 58, 19]);
  translate([2, 2, -1])
    cube([26, 54, 18]);
  // USB hole
  for (i=[0:5]){
    translate([10-i/2, -1, 15-i])    
      cube([10+i, 40, 1]);
  }
  // Battery hole
  translate([20, 10, 8]){
      cube([15, 10, 7]);
  }  
  // APA102 hole
  translate([7, 56, 7]){
      cube([17, 20, 7]);
  }
}


// For vertical button feather wing
if (printVertWing)
difference(){  
  union(){
    cube([34, 58, 2]);
    translate([15, 0, 0])
    cube([15, 29, 29]);
    
    // zipties
    translate([0,58-(20),0])
      cube([34, 20, 4]);
    
    translate([0, 0, 0])
      cube([15, 29, 4]);
  }
  
  // vertical hole  
  translate([14, 2, -1])
    cube([15, 25, 28]);
  translate([24, 2, 2])
    cube([5, 25, 30]);
}

// For button feather wing
if (printButtonWing)
difference(){  
  union(){
    cube([30, 58, 2]);
    cube([30, 26, 4]);
    translate([0,58-(20+5),0])
      cube([30, 26, 4]);
    // Left wing
    translate([3, 3, -6])
      cube([24, 1, 7]);
    // Right wing
    translate([3, 55, -6])
      cube([24, 1, 7]);
  }
  translate([15, 13, -1])
    cylinder(r1=8, r2=7, 8);
}