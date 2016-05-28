
varying mediump vec4 VarColor;

void main() {
  //gl_FragColor = vec4(0.0, 0.30, 0.60, 0.0);
  gl_FragColor = VarColor;//vec4(VarColor.x, VarColor.y, VarColor.z, 0.0);
}

/*void main( void ) {

	gl_FragColor = vec4(1.0,1.0,0.0, 1.0);

}*/
