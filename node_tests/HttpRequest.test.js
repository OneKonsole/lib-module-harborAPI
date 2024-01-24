const {expect} = require('chai');
const {HttpRequest, HttpHeader} = require('../build/Release/addon'); // Adjust the path to your built addon

describe('HttpRequestWrapper', () => {
    let request;

    beforeEach(() => {
        // Set up a new HttpRequest instance before each test
        request = new HttpRequest('http://example.com', '/path', 'GET', 80, 'body');
    });

    it('should properly initialize with constructor arguments', () => {
        expect(request.getDomain()).to.equal('http://example.com');
        expect(request.getPath()).to.equal('/path');
        expect(request.getMethod()).to.equal('GET');
        expect(request.getPort()).to.equal(80);
        expect(request.getBody()).to.equal('body');
    });

    it('should allow changing the domain', () => {
        request.setDomain('http://changed.com');
        expect(request.getDomain()).to.equal('http://changed.com');
    });

    it('should allow changing the path', () => {
        request.setPath('/newpath');
        expect(request.getPath()).to.equal('/newpath');
    });

    it('should allow changing the method', () => {
        request.setMethod('POST');
        expect(request.getMethod()).to.equal('POST');
    });

    it('should allow changing the port', () => {
        request.setPort(8080);
        expect(request.getPort()).to.equal(8080);
    });

    it('should allow changing the body', () => {
        request.setBody('new body');
        expect(request.getBody()).to.equal('new body');
    });

    it('should allow adding and retrieving headers', () => {
        const header = new HttpHeader('Content-Type', 'application/json');
        request.addHeader(header);
        const headers = request.getHeaders();
        expect(headers).to.be.an('array').with.lengthOf(1);
        expect(headers[0].getKey()).to.equal('Content-Type');
        expect(headers[0].getValue()).to.equal('application/json');
    });

    it('should retrieve a single header by key', () => {
        const header = new HttpHeader('Content-Type', 'application/json');
        request.addHeader(header);
        const retrievedHeader = request.getHeader('Content-Type');
        expect(retrievedHeader.getKey()).to.equal('Content-Type');
        expect(retrievedHeader.getValue()).to.equal('application/json');
    });

    it('should generate the correct string representation with port other than 80', () => {
        // Assuming toString generates a full HTTP request string
        request.setMethod('GET');
        request.setDomain('example.com');
        request.setPath('/path');
        request.setPort(81);
        request.addHeader(new HttpHeader('Content-Type', 'application/json'));
        request.setBody('{"key":"value"}');
        const expectedString = [
            'GET /path HTTP/1.1',
            'Host: example.com:81',
            'Content-Type: application/json',
            '',
            '{"key":"value"}'
        ].join('\r\n');
        expect(request.toString()).to.equal(expectedString);
    });

    it('should generate the correct string representation with port 80', () => {
        // Assuming toString generates a full HTTP request string
        request.setMethod('GET');
        request.setDomain('example.com');
        request.setPath('/path');
        request.setPort(80);
        request.addHeader(new HttpHeader('Content-Type', 'application/json'));
        request.setBody('{"key":"value"}');
        const expectedString = [
            'GET /path HTTP/1.1',
            'Host: example.com',
            'Content-Type: application/json',
            '',
            '{"key":"value"}'
        ].join('\r\n');
        expect(request.toString()).to.equal(expectedString);
    });

});
