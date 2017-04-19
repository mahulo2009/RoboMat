print("RoboMat!")

wifi.setmode(wifi.STATION)
wifi.sta.config("***","***")
print(wifi.sta.getip())

function motor_init(self) 
	print ("pwm.setup("..self.number..", 500, 0);");
	print ("gpio.mode("..(self.number+2)..", gpio.OUTPUT);");
	print ("gpio.write("..(self.number+2)..", gpio.LOW);");

	pwm.setup(self.number, 500, 0);
	gpio.mode(self.number+2, gpio.OUTPUT);
	gpio.write(self.number+2, gpio.LOW);
end

function motor_move(self,direction,power, time_movement)	
	duty = math.floor((1023/100) * power);

	print ("pwm.setduty(".. self.number..","..duty..")");
	print ("pwm.start("..self.number..")");

	pwm.setduty(self.number,duty);
	if (direction == 0) then
		print("gpio.write("..self.number..", gpio.LOW)");
		gpio.write(self.number+2, gpio.LOW);
	else 
		print("gpio.write("..self.number..", gpio.HIGH)");
		gpio.write(self.number+2, gpio.HIGH);
	end

	if ( not ( time_movement == 0 ) ) then
		
		tmr.alarm(self.number,time_movement,tmr.ALARM_SINGLE,function()
			print("pwm.stop("..self.number..")");
			pwm.stop(self.number)
		end)
		
	end
end

function motor_start(self)
	pwm.start(self.number);
end

motor1 = { number=1,init = motor_init,move = motor_move, start=motor_start}
motor2 = { number=2,init = motor_init,move = motor_move, start=motor_start}

function engine_init() 
	Engine.motor1.init(Engine.motor1);
	Engine.motor2.init(Engine.motor2);
end

function engine_move(direction,power, time_movement)
	Engine.motor1.move(Engine.motor1,direction,power, time_movement);
	Engine.motor2.move(Engine.motor2,direction,power, time_movement);

	Engine.motor1.start(Engine.motor1);
	Engine.motor2.start(Engine.motor2);
end

function engine_turn(direction,power, wheel_angle,time_movement)
	local power1 = 100;
	local power2 = 100;

	if (direction==0) then
		power1 = 100-25*wheel_angle;
	else
		power2 = 100-25*wheel_angle;
	end

	Engine.motor1.move(Engine.motor1,direction,power1, time_movement);
	Engine.motor2.move(Engine.motor2,direction,power2, time_movement);
end

function engine_twist(direction,power,time_movement)
	Engine.motor1.move(Engine.motor1,direction,power, time_movement);
	Engine.motor1.move(Engine.motor2,direction,power, time_movement);
end

Engine = {
	motor1=motor1,
	motor2=motor2,
	init=engine_init,
	move=engine_move,
	turn=engine_turn,
	twist=engine_twist
}

function motor (a)
	print("method " .. a[0] );
	print("motor number " .. a[1]);
	print("motor direction " .. a[2]);
	print("motor power " ..  a[3]);
	print("movement time " .. a[4]);

	motor_number = tonumber(a[1]);
	motor_direction = tonumber(a[2]);
	motor_power = tonumber(a[3]);
	movement_time = tonumber(a[4]);

	if ( (motor_number < 1) or (motor_number > 2) ) then
		print ("motor number out of range")
		return
	end

	if ( (motor_direction < 0) or (motor_direction > 1) ) then
		print ("motor direction out of range")
		return
	end

	if ( (motor_power < 0) or (motor_power > 100) ) then
		print ("motor power out of range")
		return
	end

	if (motor_number == 1) then
		Engine.motor1.move(Engine.motor1,
			motor_direction,
			motor_power,
			movement_time);
	elseif  (motor_number == 2) then
		Engine.motor2.move(Engine.motor2,
			motor_direction,
			motor_power,
			movement_time);
	end
end

function engine (a)
	print("method " .. a[0] );
	print("engine direction " .. a[1]);
	print("engine power " ..  a[2]);
	print("engine time " .. a[3]);

	engine_direction = tonumber(a[1]);
	engine_power = tonumber(a[2]);
	engine_time = tonumber(a[3]);

	if ( (engine_direction < 0) or (engine_direction > 1) ) then
		print ("engine direction out of range")
		return
	end

	if ( (engine_power < 0) or (engine_power > 100) ) then
		print ("engine power out of range")
		return
	end

	Engine.move(engine_direction,
		engine_power,
		engine_time);
end

function turn (a)
	print("method " .. a[0] );
	print("turn direction " .. a[1]);
	print("turn power " ..  a[2]);
	print("turn wheel angle " ..  a[3]);
	print("turn time " .. a[4]);

	Engine.turn(tonumber(a[1]),
		tonumber(a[2]),
		tonumber(a[3]),
		tonumber(a[4]));
end

function twist(a)
	print("method " .. a[0] );
	print("twist direction " .. a[1]);
	print("twist power " ..  a[2]);	
	print("engine time " .. a[3]);

	Engine.twist(tonumber(a[1]),
		tonumber(a[2]),
		tonumber(a[3]));
end

valid_commands = {["motor"] = motor,["engine"] = engine,["turn"] = turn, ["twist"] = twist};


Engine.init();

srv=net.createServer(net.TCP)
srv:listen(80,function(conn)
    conn:on("receive", function(sck,payload)
    	print(payload);
    	__,__,__,url =  string.find(payload, "([A-Z]+) (.+) HTTP");
		command={}
		number_commmand=0;
		for k in string.gmatch(url,"/(%w+)") do
			command[number_commmand]=k;
			number_commmand=number_commmand+1;
		end
		v =  valid_commands[command[0]];
		v(command);
    	sck:send("HTTP/1.0 200 OK\r\nAccess-Control-Allow-Origin: *\r\nContent-Length: 25\r\nContent-Type: text/html\r\n\r\n<h1> Hello, NodeMCU.</h1>");
    end)
    conn:on("sent",function(sck)
    	sck:close()
    end)
end)
