cmd_Release/obj.target/run.node := g++ -shared -pthread -rdynamic -m64  -Wl,-soname=run.node -o Release/obj.target/run.node -Wl,--start-group Release/obj.target/run/run.o -Wl,--end-group 
