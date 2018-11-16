function fatransfer(imageDir, inputFile)
    FACES = Faces('load', imageDir, 'faces.mat');

    if nargin < 2
        cap = cv.VideoCapture(0);
        frame_w = 640;
        frame_h = 480;
        cap.set('FrameWidth', frame_w);
        cap.set('FrameHeight', frame_h);
    else
        if exist(inputFile,'file') == 2
            cap = cv.VideoCapture(inputFile);
            frame_w = cap.get('FrameWidth');
            frame_h = cap.get('FrameHeight');
        else
            error('Input file not exist');
        end
    end

    S.fh = figure('units','pixels',...
        'position',[100 50 frame_w frame_h],...
        'menubar','none',...
        'name','INTRAFACE_TRACKER',...
        'numbertitle','off',...
        'resize','off',...
        'renderer','painters');

    S.ax = axes('units','pixels',...
        'position',[1 1 frame_w frame_h],...
        'drawmode','fast');

    S.im_h = imshow(zeros(frame_h,frame_w,3));
    hold on;
    S.time_h = text(frame_w-100, 40, '', 'fontsize', 15, 'color', 'c');

    set(S.fh,'KeyPressFcn',@pb_kpf);

    drawnow;

    stop_pressed = false;
    

    %% tracking and detection
    % press 'Esc' to quit
    while ~stop_pressed
        tic;
        IMG = cap.read;
        IMG = IMG(:, end:-1:1, :);
        if isempty(IMG)
            error('can not read stream from camera');
        end

        FACE = FACES.get_face(IMG);
        if ~isempty(FACE.desc)
            [similarFace, similarProps] = FACES.load_similar(FACE.desc);
            IMG = move_rect(similarFace, similarProps.prev, IMG, FACE.prev);
        end
        
        set(S.im_h, 'cdata', IMG);
        te = toc;
        update_GUI(te, FACE.pred);

        drawnow;
    end
    close;


%% Drawing and callback functions
% private function for updating drawing
    function update_GUI(te, facePred)
        % update frame/second
        set(S.time_h, 'string', sprintf('%d FPS',uint8(1/te)));
    end

    function [destImg] = move_rect(srcImg, srcRect, destImg, destRect)
        srcRect = srcRect([2, 1, 4, 3]);
        srcRect(3:4) = srcRect(3:4) + srcRect(1:2);
        rectImg = srcImg(srcRect(1):srcRect(3), srcRect(2):srcRect(4), :);
        destRect = destRect([2, 1, 4, 3]);
        rectImg = imresize(rectImg, 1 + destRect([3, 4]));        
        destRect(3:4) = destRect(3:4) + destRect(1:2);
        destImg(destRect(1):destRect(3), destRect(2):destRect(4), :) = rectImg;
    end

% private callback function
    function [] = pb_kpf(varargin)
        % Callback for pushbutton
        if strcmp(varargin{2}.Key, 'escape')==1
            stop_pressed = true;
        end
    end
end
