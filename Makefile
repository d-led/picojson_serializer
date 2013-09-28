clang	:	
			clang++ test/test.cpp test/vector_test.cpp -I Catch/single_include/ -I picojson -o catch_test
			./catch_test
