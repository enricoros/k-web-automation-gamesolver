print('1');

c = new Classifier;
c.loadDictionary("dic1");

print('2');

image = capture.capture();
image.trim(4, 5, -3, -3);

print('3');

array = image.cut(8, 8);
strings = new Array;

print('4');

for (i = 0; i < array.length; i++) {
    img = array[i];
    img.trim(2,2,-2-2);
    s = c.classify(img);
    strings[i] = s;
}

print('5');

