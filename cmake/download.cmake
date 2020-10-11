FUNCTION(GX_DOWNLOAD_FILE URL FILE_NAME)

    IF(NOT EXISTS "${FILE_NAME}.zip")
        FILE(DOWNLOAD ${URL} "${FILE_NAME}.zip" TIMEOUT 60)
        MESSAGE(STATUS "Downloading file from ${URL} ...")
    ENDIF()
    FILE(REMOVE_RECURSE "${FILE_NAME}-tmp")
    FILE(REMOVE_RECURSE "${FILE_NAME}")
    FILE(ARCHIVE_EXTRACT INPUT "${FILE_NAME}.zip" DESTINATION "${FILE_NAME}-tmp")
    FILE(GLOB INNER_DIR "${FILE_NAME}-tmp/*")
    FILE(RENAME ${INNER_DIR} ${FILE_NAME})
    FILE(REMOVE_RECURSE "${FILE_NAME}-tmp")
    MESSAGE(STATUS "Library ${FILE_NAME} has been created.")

ENDFUNCTION(GX_DOWNLOAD_FILE)