const request = require('supertest');
const app = require('./app.js');

describe('Test the routes', () => {
    test('It should have a root endpoint', async () => {
        const res = await request(app).get('/');
        expect(res.statusCode).toBe(200);
        expect(res.text).toBe("Register Now");
    });

    test('It should have a register endpoint', async () => {
        const res = await request(app).get('/register/seminar');
        expect(res.statusCode).toBe(200);
        expect(res.text).toBe("You are registered");
    });

    test('The register endpoint should handle wildcard routes', async () => {
        const res = await request(app).get('/register/otherstuff');
        expect(res.statusCode).toBe(200);
        expect(res.text).toBe("You are registered");
    });
})

describe('Test the middleware', () => {

    test('It should detect a credit card as a query param', async () => {
        const res = await request(app).get('/register/seminar')
            .query({ cardNum: '5500000000000004' });
        expect(res.statusCode).toBe(200);
        expect(res.text).toBe("Credit card detected");
    });

    test('It should not interfere with invalid card numbers', async () => {
        const res = await request(app).get('/register/seminar')
            .query({ cardNum: '5500000000000001'});
        expect(res.statusCode).toBe(200);
        expect(res.text).toBe("You are registered");     
    });

    test('It should work with multiple kinds of parameters', async () => {
        const res = await request(app).get('/register/seminar')
            .query({ notacreditcard: '5500000000000004' });
        expect(res.statusCode).toBe(200);
        expect(res.text).toBe("Credit card detected");
    });

    test('It should prevent other routes from working if card detected', async () => {
        const res = await request(app).get('/cause/error')
            .query({ notacreditcard: '5500000000000004' });
        expect(res.statusCode).toBe(200);
        expect(res.text).toBe("Credit card detected");
    });

    test('It should have error handing middleware', async () => {
        const res = await request(app).get('/cause/error');
        expect(res.statusCode).toBe(500);
        expect(res.text).toBe("An error has occurred");
    });

    test('It should have page not found middleware', async () => {
        const res = await request(app).get('/nonexistant/page');
        expect(res.statusCode).toBe(404);
        expect(res.text).toBe("Page not found");
    });
})