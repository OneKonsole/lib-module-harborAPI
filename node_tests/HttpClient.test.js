const { expect } = require('chai');
const {HttpRequest, HttpHeader, HttpClient} = require('../build/Release/addon'); // Adjust the path to your built addon

describe('HttpClientWrapper', () => {
    let client;

    before(() => {
        client = new HttpClient();
    });

    describe('executeRequest', () => {
        it('should make an HTTP GET request and receive a response', (done) => {
            // This is a hypothetical usage example since we don't have the actual implementation details
            // Replace 'http://example.com' with a valid URL and the expected response for your case
            const request = {
                url: 'http://example.com',
                method: 'GET'
            };

            client.executeRequest(request, (err, response) => {
                if (err) {
                    done(err);
                    return;
                }

                // Replace the following with actual checks for your HttpResponse
                expect(response).to.be.an('object');
                expect(response.statusCode).to.equal(200);
                expect(response.body).to.be.a('string');

                done();
            });
        });

        // Add more tests for POST, PUT, DELETE, etc.
    });

    // You can add other describe blocks to test different methods of HttpClientWrapper
});
