const { expect } = require('chai');
const { HttpResponse, HttpHeader } = require('../build/Release/addon'); // Adjust the path to your built addon

describe('HttpResponseWrapper', () => {
    let response;

    beforeEach(() => {
        // Create a new instance before each test
        response = new HttpResponse();
    });

    it('should initialize with default values', () => {
        expect(response.getStatusCode()).to.equal(200); // Assuming 200 is the default status code
        expect(response.getStatusMessage()).to.equal("OK"); // Assuming "OK" is the default status message
        expect(response.getHeaders()).to.be.an('array').that.is.empty;
        expect(response.getBody()).to.equal(""); // Assuming the default body is empty
    });

    it('should allow setting and getting the status code', () => {
        response.setStatusCode(404);
        expect(response.getStatusCode()).to.equal(404);
    });

    it('should allow setting and getting the status message', () => {
        response.setStatusMessage("Not Found");
        expect(response.getStatusMessage()).to.equal("Not Found");
    });

    it('should allow adding and retrieving headers', () => {
        const header1 = new HttpHeader('Content-Type', 'application/json');
        const header2 = new HttpHeader('Cache-Control', 'no-cache');
        response.addHeader(header1);
        response.addHeader(header2);

        const headers = response.getHeaders();
        expect(headers).to.be.an('array').with.lengthOf(2);
        expect(headers[0].getKey()).to.equal('Content-Type');
        expect(headers[0].getValue()).to.equal('application/json');
        expect(headers[1].getKey()).to.equal('Cache-Control');
        expect(headers[1].getValue()).to.equal('no-cache');
    });

    it('should allow setting and getting the body', () => {
        const bodyContent = "Response body content";
        response.setBody(bodyContent);
        expect(response.getBody()).to.equal(bodyContent);
    });

    it('should generate the correct string representation', () => {
        response.setStatusCode(200);
        response.setStatusMessage("OK");
        response.addHeader(new HttpHeader('Custom-Header', 'Custom Value'));
        response.setBody('{"message":"Hello, world!"}');

        const expectedString = [
            "HTTP/1.1 200 OK",
            "Custom-Header: Custom Value",
            "Content-Type: application/json",
            "", // This represents the end of headers and the start of the body
            '{"message":"Hello, world!"}' // Note the lack of spaces within the JSON
        ].join('\r\n');
        expect(response.toString()).to.equal(expectedString);
    });
});
