import cv2
import numpy as np
import time
import filters

class CaptureMng(object):
    def __init__(self, capture, preMng=None, isMirrorPreview=False):
        self.preMng = preMng
        self.isMirrorPreview = isMirrorPreview

        self._capture = capture
        self._channel=0
        self._enteredFrame=False
        self._frame =None
        self._imageFilename = None
        self._videoFilename = None
        self._videoEncoding = None
        self._videoWrite = None
        self._startTime = None
        self._framesElapsed = long(0)
        self._fps=None

    @property
    def channel(self):
        return self._channel

    @channel.setter
    def channel(self, v):
        if self._channel != v:
            self._channel = v
            self._frame = None

    @property
    def	frame(self):
        if	self._enteredFrame	and	self._frame	is	None:
            _,	self._frame	=	self._capture.retrieve()
            #print 'retrive ', _, self._frame
            return	self._frame
    @frame.setter
    def frame(self, v):
        self._frame = v

    @property
    def	isWritingImage(self):
        return	self._imageFilename	is	not	None

    @property
    def	isWritingVideo(self):
        return	self._videoFilename	is	not	None


    def enterFrame(self):
        """Capture the next frame, if any."""
        assert not self._enteredFrame, 'previous enterFrame() had no matching exitFram()'
        if self._capture is not None:
            self._enteredFrame = self._capture.grab()
            #print 'enter frame ', self._enteredFrame

    def exitFrame(self):
        """Draw to the window, write to files. release the frame """
        if self._frame is None:
            self._enteredFrame = False
            print 'exit first return'
            return


        if self._framesElapsed ==0:
            self._startTime = time.time()
        else:
            timeElapsed = time.time() - self._startTime
            self._fps = self._framesElapsed / timeElapsed
        self._framesElapsed +=1
        #print 'frame slapsed ' , self._framesElapsed, ' fps:', self._fps

        if self.preMng is not None:
            if self.isMirrorPreview:
                mirroredFrame = np.fliplr(self._frame).copy()
                self.preMng.show(mirroredFrame)
            else:
                self.preMng.show(self._frame)

        if self.isWritingImage:
            print 'write file'
            cv2.imwrite(self._imageFilename, self._frame)
            self._imageFilename = None

        if self.isWritingVideo:
            self._writeVideoFrame()

        self._frame = None
        self._enteredFrame = False


    def writeImage(self, filename):
        self._imageFilename = filename

    def startWriteVideo(self, filename, encoding=cv2.VideoWriter_fourcc('I','4','2','0')):
        print 'start video'
        self._videoFilename = filename
        self._videoEncoding = encoding

    def stopWriteVideo(self):
        print 'stop video'
        self._videoFilename = None
        self._videoEncoding = None
        self._videoWrite = None

    def _writeVideoFrame(self):
        if not self.isWritingVideo:
            return

        if self._videoWriter is None:
            fps = self._capture.get(cv2.CAP_PROP_FPS)
            if fps == 0.0:
                if self._framesElapsed < 20:
                    return #Wait until more frames elapese so the the estimate is more stable.
                else:
                    fps = self._fps

            size =	(int(cameraCapture.get(cv2.CAP_PROP_FRAME_WIDTH)),
                     int(cameraCapture.get(cv2.CAP_PROP_FRAME_HEIGHT)))
            self._videoWrite = cv2.VideoWrite(self._videoFilename, self._videoEncoding, fps, size)

        self._videoWrite.write(self._frame)


class	WindowMng(object):

    def	__init__(self,	windowName,	keypressCallback	=	None):
        self.keypressCallback	=	keypressCallback
        self._windowName	=	windowName
        self._isWindowCreated	=	False

    @property
    def	isWindowCreated(self):
        return	self._isWindowCreated

    def	createWindow(self):
        cv2.namedWindow(self._windowName)
        self._isWindowCreated	=	True

    def	show(self,	frame):
        cv2.imshow(self._windowName,	frame)

    def	destroyWindow(self):
        cv2.destroyWindow(self._windowName)
        self._isWindowCreated	=	False

    def	processEvents(self):
        keycode	=cv2.waitKey(10)
        if	self.keypressCallback is not None and keycode	!=	255:
            #	Discard	any	non-ASCII	info	encoded	by	GTK.
            #print "key " , keycode
            keycode	&=	0xFF
            self.keypressCallback(keycode)

class Cameo(object):
    def __init__(self):
        self._winMng = WindowMng('Cameo', self.onKeypress)
        self._captureMng = CaptureMng(cv2.VideoCapture(0), self._winMng, True)
        #self._curveFilter = filters.FindEdgesFilter()
        self._curveFilter = filters.EmbossFilter()

    def run(self):
        """run main loop. """
        self._winMng.createWindow()
        while self._winMng.isWindowCreated:
            self._captureMng.enterFrame()

            frame = self._captureMng.frame

            frame = filters.strokeEdges(frame)
            self._curveFilter.apply(frame, frame)

            self._captureMng.frame = frame

            self._captureMng.exitFrame()
            self._winMng.processEvents()

    def onKeypress(self, keycode):
        """
        space -> Take a screenshot
        tab -> Start/stop recording a screencast
        escap -> Quit
        """
        if keycode == 32: #space
            self._captureMng.writeImage('scr.png')
        elif keycode == 9: #tab
            if not self._captureMng.isWritingVideo:
                self._captureMng.startWriteVideo('scr.avi')
            else:
                self._captureMng.stopWriteVideo()
        elif keycode == 27: #esc
            self._winMng.destroyWindow()

if __name__ == "__main__":
    Cameo().run()
