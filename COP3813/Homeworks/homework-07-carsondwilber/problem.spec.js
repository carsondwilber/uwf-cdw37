const request = require('supertest');
const app = require('./app.js');

describe('Test the root path', () => {
  test('It should be the Coke Machine Menu', async () => {
    const res = await request(app).get('/');
    expect(res.statusCode).toBe(200);
    expect(res.text).toMatch(`<h1>Coke Machine Menu`);
  });
});

describe('Test vending', () => {

  test('It should accept vending requests', async () => {
    const res2 = await request(app).get('/vend/1');
    expect(res2.statusCode).toBe(200);
  });

  test('It should decrement quantity', async () => {
    // Get starting soda amount
    const res1 = await request(app).get('/report');
    expect(res1.statusCode).toBe(200);
    expect(typeof res1.body).toBe("object");
    const startingValue = res1.body[0].quantity;

    await request(app).get('/vend/1');
    const res2 = await request(app).get('/report');
    expect(res2.statusCode).toBe(200);
    expect(typeof res2.body).toBe("object");
    expect(res2.body[0].quantity).toBe(startingValue - 1);
  });

  test('It should increment vended', async () => {
    // Get starting soda amount
    const res1 = await request(app).get('/report');
    expect(res1.statusCode).toBe(200);
    expect(typeof res1.body).toBe("object");
    const startingVended = res1.body[0].vended;

    await request(app).get('/vend/1');
    const res2 = await request(app).get('/report');
    expect(res2.statusCode).toBe(200);
    expect(typeof res2.body).toBe("object");
    expect(res2.body[0].vended).toBe(startingVended + 1);
  });

  test('It should handle out of range ids', async () => {
    const res2 = await request(app).get('/vend/157');
    expect(res2.statusCode).toBe(200);
  });

  test('It should not vend past available amount', async () => {
    // Get starting soda amount
    const res1 = await request(app).get('/report');
    expect(res1.statusCode).toBe(200);
    expect(typeof res1.body).toBe("object");
    const startingValue = res1.body[0].quantity;

    // Vend entire stock out
    for (let i = 0; i < startingValue; i++) {
      await request(app).get('/vend/1');
    }

    // Verify it is empty
    await request(app).get('/vend/1');
    const res2 = await request(app).get('/report');
    expect(res2.statusCode).toBe(200);
    expect(typeof res2.body).toBe("object");
    expect(res2.body[0].quantity).toBe(0);

    // Try to vend one more, should be 0
    await request(app).get('/vend/1');
    const res3 = await request(app).get('/report');
    expect(res3.statusCode).toBe(200);
    expect(typeof res3.body).toBe("object");
    expect(res3.body[0].quantity).toBe(0);
  });

  test('It should send a 404 if no app id is provided', async () => {
    const res2 = await request(app).get('/stock');
    expect(res2.statusCode).toBe(404);
  });

});

describe('Test stocking', () => {

  test('It should accept stocking requests', async () => {
    const res2 = await request(app).get('/stock/1/15');
    expect(res2.statusCode).toBe(302);  // Should be a redirect
  });

  test('It should increment quantity', async () => {
    // Get starting soda amount
    const res1 = await request(app).get('/report');
    expect(res1.statusCode).toBe(200);
    expect(typeof res1.body).toBe("object");
    const startingValue = res1.body[0].quantity;

    await request(app).get('/stock/1/15');
    const res2 = await request(app).get('/report');
    expect(res2.statusCode).toBe(200);
    expect(typeof res2.body).toBe("object");
    expect(res2.body[0].quantity).toBe(startingValue + 15);
  });

  test('It should handle out of range ids', async () => {
    const res2 = await request(app).get('/stock/157/15');
    expect(res2.statusCode).toBe(200);
  });

  test('It should send a 404 if no app id is provided', async () => {
    const res2 = await request(app).get('/stock');
    expect(res2.statusCode).toBe(404);
  });

  test('It should send a 404 if no quantity is provided', async () => {
    const res2 = await request(app).get('/stock/157');
    expect(res2.statusCode).toBe(404);
  });
});

describe('Test receipt', () => {

  test('It should return a receipt', async () => {
    const res2 = await request(app).get('/vend/1');
    expect(res2.statusCode).toBe(200);
  });

  test('The receipt should contain the soda name', async () => {
    // Get soda name
    const res1 = await request(app).get('/report');
    expect(res1.statusCode).toBe(200);
    expect(typeof res1.body).toBe("object");
    const sodaName = res1.body[0].name;


    const res2 = await request(app).get('/vend/1');
    expect(res2.statusCode).toBe(200);
    expect(res2.text).toMatch(sodaName);

  });

  test('The receipt should contain the soda price', async () => {
    // Get soda price
    const res1 = await request(app).get('/report');
    expect(res1.statusCode).toBe(200);
    expect(typeof res1.body).toBe("object");
    const sodaPrice = res1.body[0].cost;

    const res2 = await request(app).get('/vend/1');
    expect(res2.statusCode).toBe(200);
    expect(res2.text).toMatch("$" + sodaPrice); // Note I'm assuming there's no space after the dollar sign!
  });

  test('The receipt should warn against too much caffeine', async () => {
    const res1 = await request(app).get('/report');
    expect(res1.statusCode).toBe(200);
    expect(typeof res1.body).toBe("object");
    const isCaffeinated = res1.body[0].caffeinated;

    const res2 = await request(app).get('/vend/1');
    expect(res2.statusCode).toBe(200);
    if(isCaffeinated) {
      expect(res2.text).toMatch("Be careful not to drink too much caffeine!");
    } else {
      expect(res2.text).toMatch(/^((?!(caffeine)).)*$/);

    }
  });

});
