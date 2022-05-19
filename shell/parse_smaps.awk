#!/usr/bin/awk -f

## FOR USERS
##
## Usage: parse_smaps.awk <path/for/smaps>
##        Parse smaps and get the memory usage.
##        1. Each section of memory usage (VSS, RSS, VSS).
##        2. The total used memory of VmSize, RSS, PSS, Heap and Stack.
## Node:  smaps is a file on Linux from /proc/<pid>/smaps if CONFIG_PROC_PAGE_MONITOR is set.
##
## Eg.    $ parse_smaps.awk /proc/1/smaps
##        <  /sbin/procd>: (r-xp):  VmSize = 56KB; RSS = 56KB; PSS = 56KB
##        <  /sbin/procd>: (r--p):  VmSize = 4KB;  RSS = 4KB;  PSS = 4KB
##        ..
##        ===== summary =====
##        VmSize sum: 3088 KB
##        RSS sum: 1076 KB
##        PSS sum: 715 KB
##        Heap sum: 420 KB
##        Stack sum: 20 KB
##        ===== detail (rss) =====
##        /lib/libc-X.XX.so : vm = 1264  KB rw = 24     KB  ro = 324    KB
##        [heap]            : vm = 532   KB rw = 420    KB  ro = 0      KB
##        ...

BEGIN {
    i = 0
    rss_sum = 0
    pss_sum = 0
    size_sum = 0
}
{
    # new section
    if ($1 ~ /^[[:digit:]]+/) {
        if ($6 != "") {
            if ($6 != cur_file) {
                cur_file = $6
                files[i] = $6
                i++
            }
        }
        if ($2 ~ /w/) {
            cur_perm = "w"
        } else {
            cur_perm = "r"
        }
        printf "<%50s>: (%s):", $6, $2
    }
    if ($1 ~ /^Size/) {
        printf "\tVmSize = %sKB; ", $2;
        arr[cur_file"-size"] += $2
        size_sum += $2
    }
    if ($1 ~ /^Rss/) {
        printf "\tRSS = %sKB; ", $2;
        arr[cur_file"-"cur_perm"-rss"] += $2
        rss_sum += $2
    }
    if ($1 ~ /^Pss/) {
        printf "\tPSS = %sKB\n", $2;
        arr[cur_file"-"cur_perm"-pss"] += $2
        pss_sum += $2
    }
}
END {
    printf "\n"
    printf "===== summary =====\n"
    printf "VmSize sum: %d KB\n", size_sum
    printf "RSS sum: %d KB\n", rss_sum
    printf "PSS sum: %d KB\n", pss_sum
    printf "Heap sum: %d KB\n", arr["[heap]-w-rss"] + arr["[heap]-r-rss"]
    printf "Stack sum: %d KB\n", arr["[stack]-w-rss"] + arr["[stack]-r-rss"]
    printf "===== detail (rss) =====\n"
    # sort
    i = 0
    j = 0
    for (i = 0; i < length(files) - 1; i++) {
        for (j = 0; j < length(files) - 1 - i; j++) {
            if (arr[files[j]"-size"] < arr[files[j + 1]"-size"]) {
                tmp = files[j]
                files[j] = files[j + 1]
                files[j + 1] = tmp
            }
        }
    }
    # print
    for (i = 0; i < length(files); i++) {
        file = files[i]
        printf "%-40s: vm = %-10dKB\trw = %-7dKB\tro = %-7dKB\n", file, arr[file"-size"],
               arr[file"-w-rss"], arr[file"-r-rss"]
    }
}

