# -------------------------------------------------------------------------------------------------
# CppCheck
# -------------------------------------------------------------------------------------------------
# ideally use with compile_commands.json, which is only generated with make or ninja

# Add Analyze with CppCheck target if CppCheck is installed
if(WIN32)
	# Find CppCheck executable
	find_program(CMAKE_CXX_CPPCHECK cppcheck 
		NAMES cppcheck 
		HINTS $ENV{PROGRAMFILES}/cppcheck
	)
  
  	# If CppCheck executable found
  	if(CMAKE_CXX_CPPCHECK)
	# Check CppCheck version
    	set(CPP_CHECK_CMD ${CMAKE_CXX_CPPCHECK} --version)
    	execute_process(COMMAND ${CPP_CHECK_CMD}
      		WORKING_DIRECTORY	${CMAKE_CURRENT_SOURCE_DIR}
      		RESULT_VARIABLE		CPP_CHECK_RESULT
      		OUTPUT_VARIABLE		CPP_CHECK_VERSION
      		ERROR_VARIABLE		CPP_CHECK_ERROR
		)
    
		# Check if version could be extracted
    	if(CPP_CHECK_RESULT EQUAL 0)
      		# Get number of CPU cores
      		include(ProcessorCount)
      		ProcessorCount(CPU_CORES)

      		# Append desired arguments to CppCheck
      		set(CPPCHECK_CONFIG          
        		# Using the below template will allow jumping to any found error 
				# from inside Visual Studio output window by double click
        		"--template=vs"          
        		# Use all the available CPU cores
        		"-j ${CPU_CORES}"           
        		# Only show found errors
        		"--quiet"           
        		# Desired warning level in CppCheck
        		"--enable=all"          
        		# Optional: Specified C++ version
        		"--std=c++20"           
        		# Optional: Specified platform
        		"--platform=win64"          
        		# Optional: suppression file
        		#"--suppressions-list=${CMAKE_SOURCE_DIR}/test/cppcheck_suppressions.txt"          
        		# Optional: Use inline suppressions
        		#"--inline-suppr"          
        		# Run CppCheck in this directory (relative to working directory)
        		#"src"
				# ignore files from the following directory
				#"-i${CMAKE_BINARY_DIR}"
				#
				"--project=${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.sln"
        	)
      
    		add_custom_target(CppCheck #DEPENDS my_project
        		COMMAND 			${CMAKE_CXX_CPPCHECK} ${CPPCHECK_CONFIG}
        		WORKING_DIRECTORY	${CMAKE_SOURCE_DIR}
        		COMMENT 			"Static code analysis using ${CPP_CHECK_VERSION}"
      		)
			set_target_properties(CppCheck PROPERTIES 
				FOLDER 				Tools
				EXCLUDE_FROM_ALL	TRUE
			)
            message(STATUS "CppCheck target added")


			#--------------------------------------------------------------------------------------
			# Generate HTML report
			#--------------------------------------------------------------------------------------
			find_program(CMAKE_PYTHON_EXE python)
			if(CMAKE_PYTHON_EXE)
			message(STATUS "Found python interpreter: " ${CMAKE_PYTHON_EXE})
			
				find_file(CPPCHECK_HTML cppcheck-htmlreport
					NAMES cppcheck-htmlreport.py
					HINTS ${CMAKE_SOURCE_DIR}/tools
				)
				if(CPPCHECK_HTML)
					message(STATUS "Found Cppcheck-htmlreport: " ${CPPCHECK_HTML})
					list(APPEND CPPCHECK_CONFIG_REPORT
        				# Use all the available CPU cores
        				"-j ${CPU_CORES}"
        				# Only show found errors
        				#"--quiet" 
        				# Desired warning level in CppCheck
        				"--enable=all"          
        				# Optional: Specified C++ version
        				"--std=c++20"           
        				# Optional: Specified platform
        				"--platform=win64"          
        				# Optional: suppression file
        				#"--suppressions-list=${CMAKE_SOURCE_DIR}/test/cppcheck_suppressions.txt"        
        				# Optional: Use inline suppressions
        				#"--inline-suppr"          
        				# Run CppCheck in this directory (relative to working directory)
        				#"src"
						#
						# ignore files from the following directory
						"-i${CMAKE_BINARY_DIR}"
						#
						#"--check-config"
						#
						"--project=${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.sln"
						#
						"--suppress=missingIncludeSystem"
						"--suppress=missingInclude"
						"--suppress=unmatchedSuppression"
        			)

					add_custom_target(CppCheckReport	
						COMMAND ${CMAKE_CXX_CPPCHECK} ${CPPCHECK_CONFIG_REPORT}
							--xml --xml-version=2 --output-file=test/CppCheckReport.xml	
						COMMAND python ${CPPCHECK_HTML}	
							#--source-dir=.	
							--file=test/CppCheckReport.xml	
							--title=VisualTwin
							--report-dir=test/CppCheckReport
						BYPRODUCTS	
							${CMAKE_SOURCE_DIR}/test/CppCheckReport.xml
							${CMAKE_SOURCE_DIR}/test/CppCheckReport	
						WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}	
						VERBATIM # Protect arguments to commands	
						COMMENT "Running Cppcheck and Cppcheck-htmlreport to produce HTML report."
					)
					set_target_properties(CppCheckReport PROPERTIES	
						FOLDER 				Tools
						EXCLUDE_FROM_ALL	TRUE
					)
                    message(STATUS "CppCheckReport target added")

				endif() # CPPCHECK_HTML
			endif() # Python
    	endif() # CPP_CHECK_RESULT
  	else()
		message(WARN "CppCheck was not found, install from https://cppcheck.sourceforge.io/")
	endif() # CMAKE_CXX_CPPCHECK
endif() # WIN32