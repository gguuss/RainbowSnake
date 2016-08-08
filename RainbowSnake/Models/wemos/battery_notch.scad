difference() {
  union(){
    cylinder(d1=45, d2=45, h=110);
    translate([5, -16, 0]){
      cube([20, 35, 110]);
    }
    translate([20, -16, 0]){
      cube([10, 35, 5]);
    }
    translate([20, -16, 100]){
      cube([10, 35, 5]);
    }
  }
  cylinder(d1=40, d2=40, h=120);
}