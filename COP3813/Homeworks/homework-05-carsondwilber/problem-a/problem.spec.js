const request = require('supertest');
const app = require('./app.js');

describe('Test the root path', () => {
    test('It should be Welcome Home', async () => {
        const res = await request(app).get('/');
        expect(res.statusCode).toBe(200);
        expect(res.text).toBe("Welcome Home");
    });
});

describe('Test the area method', () => {
    test('It should compute area', async () => {
        const res = await request(app).get('/area/5/100');
        expect(res.statusCode).toBe(200);
        expect(res.text).toBe("500 sq. ft.");
    });

    test('It should handle bogus data', async () => {
        const res = await request(app).get('/area/sadf/asdf');
        expect(res.statusCode).toBe(200);
        expect(res.text).toBe("NaN sq. ft.");
    });

    test('It should render the fancy page', async () => {
        const res = await request(app).get('/fancypage');
        expect(res.statusCode).toBe(200);
        expect(res.text).toMatch(`<h1>Welcome to the fancy page!</h1>`);
        expect(res.text).toMatch(`<a href="https://www.merriam-webster.com/dictionary/fancy">fancy</a>`);

    });

    test('It should have page not found handler', async () => {
        const res = await request(app).get('/nonexistant/page');
        expect(res.statusCode).toBe(404);
        expect(res.text).toBe("<h1>Page not found</h1>");
    });
});