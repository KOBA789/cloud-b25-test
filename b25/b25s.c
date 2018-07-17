#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arib25/arib_std_b25.h>
#include <arib25/b_cas_card.h>

#define MULTI2_ROUND 4

#define try(expr) \
	do { \
		if (!(expr)) { \
			fprintf(stderr, "%s:%d: assertion failed: "#expr"\n", __FILE__, __LINE__); \
			goto FINALLY; \
		} \
	} while (0)

int main() {
	int code, n;
	ARIB_STD_B25 *b25;
	B_CAS_CARD   *bcas;

	uint8_t data[64*1024];

	ARIB_STD_B25_BUFFER sbuf;
	ARIB_STD_B25_BUFFER dbuf;

	code = EXIT_FAILURE;

	try((b25 = create_arib_std_b25()) != NULL);

	try(b25->set_multi2_round(b25, MULTI2_ROUND) == 0);

	try(b25->set_strip(b25, 0) == 0);

	try(b25->set_emm_proc(b25, 0) == 0);

	try((bcas = create_b_cas_card()) != NULL);

	try(bcas->init(bcas) == 0);

	try(b25->set_b_cas_card(b25, bcas) == 0);

	while ((n = read(STDIN_FILENO, data, sizeof(data))) > 0) {
		sbuf.data = data;
		sbuf.size = n;

		try(b25->put(b25, &sbuf) == 0);

		try(b25->get(b25, &dbuf) == 0);

		if (dbuf.size > 0) {
			try(write(STDOUT_FILENO, dbuf.data, dbuf.size) == dbuf.size);
		}
	}

	try(b25->flush(b25) == 0);

	try(b25->get(b25, &dbuf) == 0);

	if (dbuf.size > 0) {
		try(write(STDOUT_FILENO, dbuf.data, dbuf.size) == dbuf.size);
	}

	code = EXIT_SUCCESS;

 FINALLY:

	if (bcas != NULL) {
		bcas->release(bcas);
		bcas = NULL;
	}

	return code;
}
