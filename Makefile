
all : build-js

build-js:
	@echo "Building JS library..."
	@node-gyp configure
	@node-gyp build
	@echo "Done."

clean :
	@echo "Cleaning..."
	@rm -rf build
	@echo "Done."