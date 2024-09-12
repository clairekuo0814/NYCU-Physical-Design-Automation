import matplotlib.pyplot as plt
import sys


def plot_blocks(L):
    cnt = 2
    while cnt < len(L):
        line = L[cnt].strip('\n')
        words = line.split(' ')
        name = words[0]
        x1 = int(words[1]) 
        y1 = int(words[2])
        x2 = int(words[3])
        y2 = int(words[4])
        plt.plot((x1,x1), (y1, y2,), 'b')
        plt.plot((x1,x2), (y2, y2,), 'b')
        plt.plot((x2,x2), (y2, y1,), 'b') 
        plt.plot((x2,x1), (y1, y1,), 'b')
        plt.text((x1+x2)/2, (y1+y2)/2, name, fontsize=5, ha='center')
        cnt += 1

if __name__ == "__main__":
    filename = "debug.txt"
    pngname = "PNG.png" 
    L = open(filename).readlines()
    plot_blocks(L)
    plt.title('Floorplan')
    #plt.show()
    plt.savefig(pngname)
