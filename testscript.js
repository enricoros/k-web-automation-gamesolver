print('1');

c = new Classifier;
c.loadDictionary("dic1");

print('2');

image = capture.capture();
print(image.width);
print(image.height);
image.trim(4, 5, -3, -3);

print('3');

array = image.split(8, 8);
print(array.length);
print(array[3]);
print(array[3].width);
strings = new Array;

print('4');

for (i = 0; i < array.length; i++) {
    img = array[i];
    img.save("test" + ig + ".png");
    img.trim(2,2,-2,-2);
    //s = c.classify(img);
    strings[i] = 1;
}

print('5');

