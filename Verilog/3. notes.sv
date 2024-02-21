module test1();
  initial begin
    $display("hello world1")
    #1ns
    $display("hello world2")
  end
  	always #1ns begin //sequential statement
      $display("hello world")
    end
endmodule

module test2(input wire clk); // clock signal
  always @(posedge clk) begin // or negedge: a toggle from 1 to 0
    $display("hello world")
  
endmodule
    
    
module test3(input wire clk); 
  wire w0;
  reg r0;
  wire w1,w2;// can be assigned by two at the same time
  reg r1,r2;
  logic l0; // cannot be assigned by two at the same time
 /*
 reg     4 states: 0,1 Z(no voltage),X(unknown)   1
 wire                                             1
 logic                                            1
 integer                                          32
 bit     2 states: 0,1                            1
 byte                       8
 shortint                   16
 int                        32
 longint                    64
 
 
 */
  assign w0 = 1'b1; //wire only continuous assignment
  assign w1 = r0 & w0;  // continuous assignment
  always @(posedge clk) begin //reg only sequential assignment
    r1 = r0 & w0;
  end
  initial r0 = 1'b1; // or procedural statement
  
endmodule


1. **Module Definition**

    module MyModule(input logic clk, input logic reset, output logic out);
        // Module body goes here
    endmodule


2. **Always Block**

    always_ff @(posedge clk or negedge reset) begin
        if (!reset) begin
            // Reset logic
        end else begin
            // Sequential logic
        end
    end

    //This is an always block that triggers on the positive edge of the clock or the negative edge of the reset signal.

3. **If-Else Statement**

    if (condition) begin
        // If condition is true
    end else begin
        // If condition is false
    end


4. **Case Statement**

    case (expression)
        value1: statement1;
        value2: statement2;
        default: default_statement;
    endcase


5. **For Loop**

    for (i=0; i<10; i=i+1) begin
        // Loop body
    end


6. **Functions**

    function automatic [return_type] function_name ([arguments]);
        // Function body
    endfunction


7. **Tasks**

    task automatic task_name ([arguments]);
        // Task body
    endtask


8. **Interface**
      
    interface MyInterface;
        logic signal1;
        logic signal2;
    endinterface




    
    
// Enumeration
typedef enum {RED, GREEN, BLUE} color_e;

// String
string str = "Hello, World!";

// Structure
typedef struct {
  logic [7:0] addr;
  logic [31:0] data;
} bus_t;

// Fixed Size Array
logic [7:0] array_fixed[10];

// Variable Size Array
logic [7:0] array_var[];
array_var = new[10];
    
    int array[$] // queue
    /*
    pop/push_back/front
    insert
    delete(index)
    size
    sum
    min/max
    unique
    
    */
    int array[*] // associativ e array 离散化map

// Function and Tasks
function automatic int add(int a, int b);
  return a + b;
endfunction
    
function automatic void add();
  int a;
  a = a + 1;
endfunction // a=1,a=1,a=1 a is software local variable
    
function void add();
  int a;
  a = a + 1;
endfunction // a=1,a=2,a=3 a is hardware signal

    // task has no return value
    //task can call function but not vice versa
    task automatic delay();
      #10;
    endtask 

    //pass argument by value
    task t(input int a);
      a++;
      a++;
    endtask
    int b;
    initial begin
      b = 1;
      t(b); // b will still be 1
    end
    
    
    task t(output int a);
      a++;
      a++;
    endtask
    int b;
    initial begin
      b = 1;
      t(b); // b will be 2
    end
    
    task t(input int a,b,ouput int r);
      r = a+b;
    endtask
    
    //pass argument by ref
    function void func(ref int a)
    endfunction
    
// Threads
fork
  #10ns xxxA
  #20ns xxxB
  delay();
join  // the current thread
join_any //all threads
disable fork; // disable active threads at exit
xxxC   
    
    

// Events
event e;
  ->(ev) //trigger event
wait(e);
wait(clk == 1'b1)
  //@ : events
  @(clk) // wait for clk
  @(posedge clk, negedge clk) // wait for posedge clk or negedge clk
  @(posedge clk or negedge clk)

  
  
  
  
// Class Member
class MyClass;
  int member;
  function int get();
    return 1;
  endfunction
endclass

// Class Pointer
  MyClass class_ptr = new();
  for (int i = 0;i<3;i++) begin
    // MyClass class_ptr = new(); // if; q[0]->i=0;q[1]->i=1
    class_ptr.i = i;
    q.push_back(a);
  end
  // all q[0] q[1] ... point to current i=2
  

// Covergroup and Coverpoints
covergroup cg;
  coverpoint class_ptr.member;
endcovergroup

// Interface
interface MyInterface;
  logic signal;
endinterface

// Virtual Interface
virtual MyInterface vif;

// Nonblocking
always @(posedge clk) begin
  a <= b;
end

// Assertion
assert (a == b) else $error("Assertion failed!");

// Concurrent Assertion
always @(posedge clk) begin
  assert property (@(posedge clk) disable iff (!reset) a |-> ##1 b);
end

// Cross Modules Interface
bind my_module my_interface intf(.clk(clk), .rst(rst));

// Compiler Directives
`ifdef DEBUG
  $display("Debug mode");
`endif

// Package
package MyPackage;
  int constant = 42;
endpackage
