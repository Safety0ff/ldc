#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

macro(git_get_version TAG REFSPEC HASH WT_MODIFIED)
	if (NOT GIT_FOUND)
		find_package (Git QUIET)
	endif ()

	if (GIT_FOUND)
		execute_process (
			COMMAND ${GIT_EXECUTABLE} describe --exact-match --tags
			WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
			OUTPUT_VARIABLE ${TAG}
			ERROR_QUIET
			OUTPUT_STRIP_TRAILING_WHITESPACE
		)
		execute_process (
			COMMAND ${GIT_EXECUTABLE} symbolic-ref HEAD
			WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
			OUTPUT_VARIABLE ${REFSPEC}
			ERROR_QUIET
			OUTPUT_STRIP_TRAILING_WHITESPACE
		)
		execute_process (
			COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
			WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
			OUTPUT_VARIABLE ${HASH}
			ERROR_QUIET
			OUTPUT_STRIP_TRAILING_WHITESPACE
		)
		execute_process (
			COMMAND ${GIT_EXECUTABLE} diff --quiet --ignore-submodules HEAD
			WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
			RESULT_VARIABLE ${WT_MODIFIED}
			OUTPUT_QUIET
			ERROR_QUIET
		)
	else ()
		set (${TAG} "GIT-NOTFOUND")
		set (${REFSPEC} "GIT-NOTFOUND")
		set (${HASH} "GIT-NOTFOUND")
		set (${WT_MODIFIED} "GIT-NOTFOUND")
	endif()
endmacro()
