#include <assert.h>
#include <stdio.h>
#include <unistd.h>

#include <cutils/time.h>
#include <cutils/timeout_list.h>

// Test a timeout-list where retrieved entries are not older than 100ms
#define TIMEOUT_MS 100

int
main(void)
{
    int err = 0;

    // Init a timeout-list with entry size of 8 bytes
    tolist_ctx_t *tolctx = timout_list_init(TIMEOUT_MS, sizeof(uint64_t));

    // Push an entry
    uint64_t ts0 = gettsc();
    err = timout_list_put(tolctx, &ts0);
    assert(err == 0);
    usleep(TIMEOUT_MS * 1000);

    // Push second entry
    // By this time first entry will be obsolete due to sleep above
    uint64_t ts1 = gettsc();
    err = timout_list_put(tolctx, &ts1);
    assert(err == 0);

    // Verify that the list returns only one entry and that's the second one
    uint64_t buf[2] = { 0 };
    err = timout_list_get(tolctx, buf, sizeof(buf));
    assert(err == sizeof(ts1) && buf[0] == ts1);

    // Destroy the timeout-list
    timeout_list_fini(tolctx);

    // Gets here only if above test passes
    printf("PASSED\n");
    return 0;
}
