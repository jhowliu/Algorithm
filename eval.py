import sys
import numpy as np

if __name__ == '__main__':
    # Load answer
    tmp = open(sys.argv[1]).readlines()
    ans = np.array(sorted(map(lambda x: int(x.replace('\n', '')), tmp)[1:]))

    tmp = open(sys.argv[2]).readlines();
    result = np.array(map(lambda x: int(x.replace('\n', '')), tmp))

    print "Error = " + str(sum((result-ans)!=0))
