open Revery;
open Revery.UI;
open Revery.UI.Components;

module AnimatedText = {
  module Styles = {
    open Style;

    let text = (~yOffset) => [
      color(Colors.white),
      fontFamily("Roboto-Regular.ttf"),
      fontSize(24.),
      transform([Transform.TranslateY(yOffset)]),
    ];
  };

  let%component make = (~delay: Time.t, ~text: string, ()) => {
    let%hook (yOffset, _state, _reset) =
      Hooks.animation(
        Animation.animate(Time.ms(500))
        |> Animation.delay(delay)
        |> Animation.ease(Easing.easeOut)
        |> Animation.tween(50., 0.),
      );

    let%hook (animatedOpacity, _state, _reset) =
      Hooks.animation(
        Animation.animate(Time.seconds(1))
        |> Animation.delay(delay)
        |> Animation.ease(Easing.easeOut)
        |> Animation.tween(0., 1.),
      );

    <Opacity opacity=animatedOpacity>
      <Padding padding=8>
        <Text style={Styles.text(~yOffset)} text />
      </Padding>
    </Opacity>;
  };
};

module SimpleButton = {
  module Styles = {
    open Style;

    let button = [
      backgroundColor(Color.rgba(1., 1., 1., 0.1)),
      border(~width=2, ~color=Colors.white),
      margin(16),
    ];

    let text = [
      color(Colors.white),
      fontFamily("Roboto-Regular.ttf"),
      fontSize(20.),
    ];
  };

  let%component make = () => {
    let%hook (count, setCount) = React.Hooks.state(0);
    let increment = () => setCount(count => count + 1);

    let text = "Click me: " ++ string_of_int(count);
    <Clickable onClick=increment>
      <View style=Styles.button>
        <Padding padding=4> <Text style=Styles.text text /> </Padding>
      </View>
    </Clickable>;
  };
};

module NativeFileExamples = {
  let%component make = () => {
    let%hook (fileListOpt, setFileListOpt) = Hooks.state(None);
    let%hook (allowMultiple, setAllowMultiple) = Hooks.state(false);
    let%hook (showHidden, setShowHidden) = Hooks.state(false);

    let openFile = () => {
      let o =
        Dialog.openFiles(
          ~allowMultiple,
          ~showHidden,
          ~title="Revery Open File Example",
          ~buttonText=
            allowMultiple ? "Open file(s) in Revery" : "Open file in Revery",
          (),
        );
      setFileListOpt(_ => o);
    };

    let optionStyle = Style.[color(Colors.white)];

    let titleStyle = Style.[color(Colors.white)];

    let containerStyle =
      Style.[
        position(`Absolute),
        justifyContent(`Center),
        alignItems(`Center),
        bottom(0),
        top(0),
        left(0),
        right(0),
      ];

    <View style=containerStyle>
      <Text style=titleStyle fontSize=20. text="Open Files and Folders" />
      <Row>
        <Text style=optionStyle text="Allow multiple?" />
        <Checkbox
          checked=allowMultiple
          checkedColor=Colors.green
          onChange={() => setAllowMultiple(am => !am)}
        />
      </Row>
      <Row>
        <Text style=optionStyle text="Show hidden?" />
        <Checkbox
          checked=showHidden
          checkedColor=Colors.green
          onChange={() => setShowHidden(sh => !sh)}
        />
      </Row>
      <Button title="Open File" onClick=openFile />
      {switch (fileListOpt) {
       | Some(fileList) =>
         fileList
         |> Array.to_list
         |> React.listToElement
       | None => <View />
       }}
    </View>;
  };
};

let render = () => <NativeFileExamples />;

let main = (~win) => {
  module Styles = {
    open Style;

    let container = [
      position(`Absolute),
      justifyContent(`Center),
      alignItems(`Center),
      bottom(0),
      top(0),
      left(0),
      right(0),
    ];

    let inner = [flexDirection(`Row), alignItems(`FlexEnd)];
  };

  <View style=Styles.container>
    render()
  </View>;
};

let init = app => {
  Revery.App.initConsole();

  Timber.App.enable();
  Timber.App.setLevel(Timber.Level.perf);

  let win = App.createWindow(app, "Welcome to Revery!");

  let _: Revery.UI.renderFunction = UI.start(win, <main win />);
  ();
};

App.start(init);
