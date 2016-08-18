// Shell!
outerX = 42;
outerY = 32;
outerZ = 32;

// BUTTON!
buttonRadius = 7;
buttonOffsetX = 16;

// Wall!
wallSize = 2;

// Ziptie
zipTieSize = 8;
zipTieOffset = 13;

// parts
drawHead = true;
drawTinyHead = false;
drawFoot = false;

if (drawHead) {
  
  translate([30, -3, 10])
    rotate([0, 0, 90])  
  union() {
    difference(){
      cube([outerX, outerY, outerZ]);
      
      // Hollow out
      union() {
        translate([0,0,-7]);
        innerCube(outerX, outerY, outerZ, wallSize);
      }
      
      // USB hole!
      for (i=[3:9]){
        translate([-1, 11-i/2, 9-i])    
          cube([5, 10+i, 1]);
      }
      
      // Button!
      translate([buttonOffsetX + buttonRadius/2, outerY / 2, outerZ - (wallSize * 3)])
        cylinder(r1=buttonRadius+1, r2=buttonRadius-1, h=wallSize * 4);
      
      // Ziptie!
      translate([outerX - zipTieOffset, -1, outerZ - wallSize])
        cube([zipTieSize, outerY + 2, wallSize * 2]);
    }


    // keep pins
    translate([outerX - 9, outerY / 2 - 6, 10]) 
      cube([5, 12, outerZ - 13]);
    translate([1, outerY / 2 - 6, 17]) 
      cube([5, 12, outerZ - 18]);
  }
}

if (drawTinyHead) {
  
  translate([30, -3, 10])
    rotate([0, 0, 90])  
  union() {
    difference(){
      cube([outerX, outerY, outerZ -17]);
      
      // Hollow out
      union() {
        translate([0,0,-7]);
        innerCube(outerX, outerY, outerZ - 17, wallSize);
      }
      
      // USB hole!
      for (i=[3:9]){
        translate([-1, 11-i/2, 9-i])    
          cube([5, 10+i, 1]);
      }
      
      // Ziptie!
      translate([outerX - zipTieOffset, -1, outerZ - wallSize - 17])
        cube([zipTieSize, outerY + 2, wallSize * 2]);
    }
  }
}

// NOTE: places cube 1 unit lower about Z for clean differences.
module innerCube(x, y, z, wallSize) {  
  translate([wallSize, wallSize, -1])
    cube([x - (wallSize * 2), y - (wallSize * 2), z - (wallSize * 2) + 1]);
}

if (drawFoot){
  difference(){
    union(){ 
      difference() {
        union() {
          translate([-2, -3, -4])
            cube([32, 42, 8]);         
        }
        cube([27, 35, 16]);
          
        // Cord slot
        translate([7, 20, 2])
            cube([12, 50, 6]);    
      }
      difference() {
        union(){
          // flaps
          union(){ 
          translate([1, 0, -4])
              cube([5, 1, 10]);
          translate([21, 0, -4])
              cube([5, 1, 10]);
          }
          translate([1, 35, -4])
              cube([25, 1, 10]);
        }
        // Cord slot
        translate([7, 20, 2])
            cube([12, 50, 6]);   
        
 
      }

      //
      translate([13, 11, -1])
            cylinder(d1=6, d2=6, h=11);
      
      translate([13, 25, -1])
            cylinder(d1=6, d2=6, h=11);
    }
    
    // USB slot
    translate([7, -20, 3])
        cube([12, 21, 6]);  
    
    // Ziptie!
    translate([-2, 38 - zipTieOffset, -2 - wallSize*2])
      cube([outerX + 3, zipTieSize, 1+ (wallSize * 2)]);
  }
}