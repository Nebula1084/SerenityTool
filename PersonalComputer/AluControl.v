module AluControl(
input[1:0] op,
input[5:0] Func,
output[2:0] operation
	);
assign operation = 
	(op[1] == 0)? {op[0],2'b10}:(
		(Func[5:2]==4'b1000)? {Func[1],2'b10}:( //add,addu,sub,subu
			(Func[5:1]==5'b10010)? {2'b00,Func[0]}:(	//and,or
				(Func[5:0]==6'b101010)? 3'b111 : 3'b011	//slt,default
			)
		)
	);

endmodule
