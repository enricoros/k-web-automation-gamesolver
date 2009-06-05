// GAME parameters
var b_w = 39, b_h = 40, n_x = 8, n_y = 8;

cc = new ColorClassifier;
cc.setDictionary("bej-dict");

image = capture.capture().copy(191, 126, n_x * b_w, n_y * b_h);

blocks = image.split(n_x, n_y);
for (i = 0; i < blocks.length; i++) {
    blocks[i].trim(8, 8, -8, -8);
    blocks[i].sym = cc.classify(blocks[i], false);
}

