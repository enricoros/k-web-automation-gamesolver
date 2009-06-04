c = new Classifier;
c.loadDictionary("dic1");

var bw = 39, bh = 39, nbx = 8, nby = 8;

// 130, 263
image = capture.capture().copy(190, 130, nbx * bw, nby * bh);
image.show();

array = image.split(nbx, nby);
strings = new Array;

print('4');

for (i = 0; i < array.length; i++) {
    img = array[i];
    img.trim(4, 4, -4, -4);
    s = c.classify(img);
    print(i + ", " + s);
    strings[i] = 1;
}

print('5');

