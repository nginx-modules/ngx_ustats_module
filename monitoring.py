#!/usr/bin/python

import sys, json, urllib2, re, os, time
from optparse import OptionParser


def get_upstream_name(upstream):
    return re.sub(r' .*', '', upstream)


def upstream_list(ustats):
    for name, backend in ustats.items():
        for upstream in backend:
            if type(upstream) is list:
                print name + "_" + get_upstream_name(upstream[0])


def read_file(fname):
    if os.path.isfile(fname):
        try:
            f = open(fname, 'r')
            data = f.read()
        except IOError as e:
            print "Error on persistence read: ", e
            sys.exit(1)
        except:
            print "Unexpected error on persistence read: ", sys.exc_info()[0]
            sys.exit(1)
    else:
        return None

    try:
        json_data = json.loads(data)
        return json_data
    except:
        print "Unexpected error on parsing persistence data: ", sys.exc_info()[
            0]
        print "Consider removing: ", fname
        sys.exit(1)


def write_file(fname, data):
    try:
        f = open(fname, 'w')
        json_data = json.dumps(data)
        f.write(json_data)
    except IOError as e:
        print "Error on persistence write: ", e
        sys.exit(1)
    except:
        print "Unexpected error on persistence write: ", sys.exc_info()[0]
        sys.exit(1)


def upstream_500(ustats, options):
    upstream = None
    for name, backend in ustats.items():
        for upstream_item in backend:
            if type(upstream_item) is list:
                item_name = name + "_" + get_upstream_name(upstream_item[0])
                if item_name == options.upstream:
                    upstream = upstream_item
                    break

    if upstream is None:
        print "Upstream is not found: ", options.upstream
        sys.exit(1)

    start_time = upstream[13]
    count_500 = upstream[5]
    if options.verbose:
        print start_time, count_500

    fname = options.path + '/ustats_' + options.upstream
    upstream_persistence = read_file(fname)
    current_tick = int(int(time.time()) / options.tick) * options.tick

    if start_time == "0000-00-00 00:00:00":
        start_timestamp = 0
    else:
        start_timestamp = int(
            time.mktime(time.strptime(start_time, '%Y-%m-%d %H:%M:%S')))

    if upstream_persistence is None or upstream_persistence[
            'start'] != start_time:
        if upstream_persistence is None:
            restart = []
        else:
            restart = upstream_persistence['restart']

        if start_timestamp > 0 and (len(restart) == 0 or
                                    restart[-1] != start_timestamp):
            restart.append(start_timestamp)

        upstream_persistence = {
            'start': start_time,
            'restart': restart,
            'ticks': [[current_tick, count_500]]
        }
    elif upstream_persistence['ticks'][-1][0] == current_tick:
        upstream_persistence['ticks'][-1][1] = count_500
    else:
        upstream_persistence['ticks'] = [
            tick for tick in upstream_persistence['ticks']
            if tick[0] > (int(time.time()) - options.tick * options.tick_num)
        ]
        upstream_persistence['ticks'].append([current_tick, count_500])

    restart = upstream_persistence['restart']
    restart = [
        ts for ts in restart
        if ts > (int(time.time()) - options.tick * options.tick_num)
    ]
    upstream_persistence['restart'] = restart

    if options.verbose:
        print upstream_persistence
    write_file(fname, upstream_persistence)

    if len(upstream_persistence['ticks']) < 2:
        print 0
    else:
        print(upstream_persistence['ticks'][-1][1] -
              upstream_persistence['ticks'][0][1])


def upstream_restart(options):
    fname = options.path + '/ustats_' + options.upstream
    upstream_persistence = read_file(fname)
    if options.verbose:
        print upstream_persistence
    if upstream_persistence is None:
        print 0
    else:
        restart = upstream_persistence['restart']
        restart = [
            ts for ts in restart
            if ts > (int(time.time()) - options.tick * options.tick_num)
        ]
        print len(restart)


def main(argv):
    parser = OptionParser()
    parser.add_option(
        "-a",
        "--action",
        dest="action",
        help="Action value (upstream-list, upstream-500, upstream-restart)")
    parser.add_option("-u", "--url", dest="url", help="Ustats data source url")
    parser.add_option(
        "-t",
        "--tick",
        dest="tick",
        help="Stat tick length in seconds",
        type="int")
    parser.add_option(
        "-n",
        "--tick-num",
        dest="tick_num",
        help="Stat tick count to sum",
        type="int")
    parser.add_option(
        "-s", "--upstream", dest="upstream", help="Upstream to get stat data")
    parser.add_option(
        "-p", "--path", dest="path", help="Path to store temp data")
    parser.add_option(
        "-v",
        "--verbose",
        dest="verbose",
        help="Output more data",
        action="store_true")

    (options, args) = parser.parse_args()

    if options.url is None:
        print parser.format_help()

    if options.action == "upstream-500" and (options.tick < 1 or
                                             options.tick_num < 1):
        print "tick and tick-num should be positive integers"
        sys.exit(1)

    #fetching ustats data
    if options.action != "upstream-restart":
        try:
            response = urllib2.urlopen(options.url)
            json_data = response.read()
            if options.verbose:
                print json_data
        except urllib2.URLError as e:
            print "URLError on fetching ustats data: ", e
            sys.exit(1)
        except:
            print "Unexpected error on fetching ustats data: ", sys.exc_info()[
                0]
            sys.exit(1)

        #parsing json output
        try:
            ustats = json.loads(json_data)
            if options.verbose:
                print ustats
        except:
            print "Unexpected error on fetching ustats data: ", sys.exc_info()[
                0]
            sys.exit(1)

    if options.action == "upstream-list":
        upstream_list(ustats)
    elif options.action == "upstream-500":
        upstream_500(ustats, options)
    elif options.action == "upstream-restart":
        upstream_restart(options)
    else:
        print "Unrecognized action ", options.action
        sys.exit(1)


if __name__ == "__main__":
    main(sys.argv[1:])
