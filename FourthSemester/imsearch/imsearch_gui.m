function varargout = imsearch_gui(varargin)
    % IMSEARCHGUI MATLAB code for imsearchGUI.fig
    %      Create imsearch GUI.

    % Edit the above text to modify the response to help imsearchGUI

    % Last Modified by GUIDE v2.5 02-Aug-2015 15:21:50

    % Begin initialization code - DO NOT EDIT
    gui_Singleton = 1;
    gui_State = struct('gui_Name',       mfilename, ...
                       'gui_Singleton',  gui_Singleton, ...
                       'gui_OpeningFcn', @imsearchGUI_OpeningFcn, ...
                       'gui_OutputFcn',  @imsearchGUI_OutputFcn, ...
                       'gui_LayoutFcn',  [] , ...
                       'gui_Callback',   []);
    if nargin && ischar(varargin{1})
        gui_State.gui_Callback = str2func(varargin{1});
    end

    if nargout
        [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
    else
        gui_mainfcn(gui_State, varargin{:});
    end
    % End initialization code - DO NOT EDIT
end


% --- Executes just before imsearchGUI is made visible.
function imsearchGUI_OpeningFcn(hObject, ~, handles, varargin)
    % This function has no output args, see OutputFcn.
    % hObject    handle to figure
    % handles    structure with handles and user data (see GUIDATA)
    % varargin   command line arguments to imsearchGUI (see VARARGIN)

    if isempty(varargin) 
        error('Please, specify image dir.');
    end
    
    if exist(varargin{1}, 'dir') == 0
        error('This dir doesn''t exist.');
    else
        imageDir = varargin{1};
    end
    
    if size(varargin, 1) < 3
        bagFilename = 'bag.mat';
    else
        bagFilename = varargin{3};
    end
    
    if size(varargin, 1) < 2
        gistFilename = 'gist.mat';
    else
        gistFilename = varargin{2};
    end
    
    handles.gist = Gist('load', imageDir, gistFilename);
    handles.bag = Bag('load', imageDir, bagFilename);
    
    % Choose default command line output for imsearchGUI
    handles.output = hObject;

    % Update handles structure
    guidata(hObject, handles);

    % UIWAIT makes imsearchGUI wait for user response (see UIRESUME)
    % uiwait(handles.imsearchGUI);
end


% --- Outputs from this function are returned to the command line.
function varargout = imsearchGUI_OutputFcn(~, ~, handles) 
    % varargout  cell array for returning output args (see VARARGOUT);
    % hObject    handle to figure
    % handles    structure with handles and user data (see GUIDATA)


    % Get default command line output from handles structure
    varargout{1} = handles.output;
end


% --- Executes on button press in gistButton.
function gistButton_Callback(~, ~, handles) %#ok<DEFNU>
    % hObject    handle to gistButton (see GCBO)
    % handles    structure with handles and user data (see GUIDATA)
    
    nearImages = handles.gist.find(handles.imagePathname, 16);
	show_images(handles.gist.ImagePathnames, nearImages);
end


% --- Executes on button press in siftButton.
function bagButton_Callback(~, ~, handles) %#ok<DEFNU>
    % hObject    handle to siftButton (see GCBO)
    % handles    structure with handles and user data (see GUIDATA)
    
    nearImages = handles.bag.find(handles.imagePathname, 16);
    show_images(handles.gist.ImagePathnames, nearImages);
end


% --------------------------------------------------------------------
function openImage_ClickedCallback(hObject, ~, handles) %#ok<DEFNU>
    % hObject    handle to openImage (see GCBO)
    % handles    structure with handles and user data (see GUIDATA)

    [filename, path] = uigetfile({'*.jpg'}, 'Open Image');
    if filename
        gistButton = findobj('Tag', 'gistButton');
        set(gistButton, 'Enable', 'on');
        bagButton = findobj('Tag', 'bagButton');
        set(bagButton, 'Enable', 'on');
        
        handles.imagePathname = fullfile(path, filename);
        handles.img = imread(handles.imagePathname);
        imshow(handles.img);
        guidata(hObject, handles);
    end
end
