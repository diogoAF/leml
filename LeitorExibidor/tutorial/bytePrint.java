public class bytePrint {
 
        public static void main(String[] args) {
               
              
                 int data[] = new int[] {1,2,3,4,-5,6,7,-8 };
				 
                 byte b1 = 100;
                 byte b2 = 20;
				 byte b3 = -20;
				 byte b4 = -100;
				 
				 short i1 = 30000;
                 short i2 = 20;
				 short i3 = -20;
				 short i4 = -30000;
				 
				 long l1 = 9123372036854775808L;
                 long l2 = 20L;
				 long l3 = -20L;
				 long l4 = -9123372036854775808L;
				 
				 float f1 = 100.1f;
                 float f2 = 20.2f;
				 float f3 = -20.2f;
				 float f4 = -100.1f;
				 
				 double d1 = 123.4;
                 double d2 = 1.234e2;
				 double d3 = -20.2;
				 double d4 = -123.4;
                 
				
				 
                System.out.println("Value of byte variable b1 is :");
				System.out.println(b1);
                System.out.println("Value of byte variable b2 is :");
				System.out.println(b2);
				System.out.println("Value of byte variable b3 is :");
				System.out.println(b3);
                System.out.println("Value of byte variable b4 is :");
				System.out.println(b4);
				
				System.out.println("Value of short variable i1 is :");
				System.out.println(i1);
                System.out.println("Value of short variable i2 is :");
				System.out.println(i2);
				System.out.println("Value of short variable i3 is :");
				System.out.println(i3);
                System.out.println("Value of short variable i4 is :");
				System.out.println(i4);
				
				System.out.println("Value of long variable l1 is :");
				System.out.println(l1);
                System.out.println("Value of long variable l2 is :");
				System.out.println(l2);
				System.out.println("Value of long variable l3 is :");
				System.out.println(l3);
                System.out.println("Value of long variable l4 is :");
				System.out.println(l4);
				
				System.out.println("Value of float variable f1 is :");
				System.out.println(f1);
                System.out.println("Value of float variable f2 is :");
				System.out.println(f2);
				System.out.println("Value of float variable f3 is :");
				System.out.println(f3);
                System.out.println("Value of float variable f4 is :");
				System.out.println(f4);
				
				System.out.println("Value of double variable d1 is :");
				System.out.println(d1);
                System.out.println("Value of double variable d2 is :");
				System.out.println(d2);
				System.out.println("Value of double variable d3 is :");
				System.out.println(d3);
                System.out.println("Value of double variable d4 is :");
				System.out.println(d4);
				
				System.out.println("Value of array variable data is :");
				for(int i = 0; i < data.length; i++) {
					System.out.println(data[i]);
				}
				System.out.println( data.length);
        }
}
 
/*
Output would be
Value of byte variable b1 is :100
Value of byte variable b1 is :20
*/