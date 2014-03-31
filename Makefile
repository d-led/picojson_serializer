clang	:	
			clang++ test/test.cpp test/nonintrusive_test.cpp test/vector_test.cpp test/custom_converter_example.cpp test/map_test.cpp test/projections_test.cpp -I Catch/single_include/ -I picojson -o catch_test
			./catch_test
