#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include "mbedtls/md.h"
#include "sha.h"


void hash(char* payload, byte *shaResult) {
	
	// char *payload = "Hello SHA 256 from ESP32learning";
	mbedtls_md_context_t ctx;
	mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;
	
	const size_t payloadLength = strlen(payload);
	
	mbedtls_md_init(&ctx);
	mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 0);
	mbedtls_md_starts(&ctx);
	mbedtls_md_update(&ctx, (const unsigned char *) payload, payloadLength);
	mbedtls_md_finish(&ctx, shaResult);
	mbedtls_md_free(&ctx);

	// char* extension = "HEY";
    // char* final;
    // final = malloc(strlen(payload)+1+strlen(extension));
    // strcpy(final, payload);
    // strcat(final, extension);
}







